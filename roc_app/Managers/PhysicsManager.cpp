#include "stdafx.h"

#include "Managers/PhysicsManager.h"
#include "Core/Core.h"
#include "Elements/Element.h"
#include "Elements/Collider.h"
#include "Elements/Model/Model.h"

#include "Managers/ConfigManager.h"
#include "Managers/ElementManager.h"
#include "Elements/Model/Skeleton.h"

namespace ROC
{

const float g_physicsDefaultTimestep = 1.f / 120.f;
const int g_physicsDefaultSubstepsCount = 10;

}

ROC::PhysicsManager::PhysicsManager(Core *p_core) : Manager(p_core)
{
    m_enabled = true;
    m_floorBody = nullptr;
}

void ROC::PhysicsManager::Start()
{
    if(!IsActive())
    {
        m_broadPhase = new btDbvtBroadphase();
        m_collisionConfig = new btDefaultCollisionConfiguration();
        m_dispatcher = new btCollisionDispatcher(m_collisionConfig);
        m_solver = new btSequentialImpulseConstraintSolver();

        m_dynamicWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadPhase, m_solver, m_collisionConfig);
        m_dynamicWorld->setGravity(btVector3(0.f, -9.8f, 0.f));

        unsigned int l_fpsLimit = GetCore()->GetConfigManager()->GetFPSLimit();
        m_timeStep = (l_fpsLimit == 0U) ? g_physicsDefaultTimestep : (1.5f / static_cast<float>(l_fpsLimit));

        SetFloorEnabled(true);
    }

    Manager::Start();
}

void ROC::PhysicsManager::Stop()
{
    if(IsActive())
    {
        m_enabled = false;
        if(m_floorBody)
        {
            m_dynamicWorld->removeRigidBody(m_floorBody);
            delete m_floorBody->getMotionState();
            delete m_floorBody;
        }
        m_floorBody = nullptr;

        delete m_dynamicWorld;
        delete m_solver;
        delete m_dispatcher;
        delete m_collisionConfig;
        delete m_broadPhase;

        m_collisions.clear();
    }

    Manager::Stop();
}

void ROC::PhysicsManager::SetPhysicsEnabled(bool p_value)
{
    if(!IsActive()) return;

    if(m_enabled != p_value)
    {
        m_enabled = p_value;
        ROC::Skeleton::SetPhysicsEnabled(m_enabled);
    }
}

bool ROC::PhysicsManager::GetPhysicsEnabled() const
{
    return m_enabled;
}

void ROC::PhysicsManager::SetFloorEnabled(bool p_value)
{
    if(!IsActive()) return;

    if(p_value && (m_floorBody == nullptr))
    {
        btStaticPlaneShape *l_groundShape = new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), 0.f);
        btDefaultMotionState *l_groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.f, 0.f, 0.f, 1.f), btVector3(0.f, 0.f, 0.f)));
        btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0.f, l_groundMotionState, l_groundShape);
        m_floorBody = new btRigidBody(groundRigidBodyCI);
        m_dynamicWorld->addRigidBody(m_floorBody);
    }
    else if(!p_value && (m_floorBody != nullptr))
    {
        m_dynamicWorld->removeRigidBody(m_floorBody);
        delete m_floorBody->getMotionState();
        delete m_floorBody;
        m_floorBody = nullptr;
    }
}

bool ROC::PhysicsManager::GetFloorEnabled() const
{
    return (m_floorBody != nullptr);
}

void ROC::PhysicsManager::SetGravity(const glm::vec3 &p_grav)
{
    if(!IsActive()) return;

    btVector3 l_grav(p_grav.x, p_grav.y, p_grav.z);
    m_dynamicWorld->setGravity(l_grav);

    btCollisionObjectArray &l_objectArray = m_dynamicWorld->getCollisionObjectArray();
    for(int i = 0, j = l_objectArray.size(); i < j; i++)
    {
        btRigidBody *l_body = btRigidBody::upcast(l_objectArray[i]);
        if(l_body)
        {
            l_body->setGravity(l_grav);
            l_body->activate(true);
        }
    }
}

void ROC::PhysicsManager::GetGravity(glm::vec3 &p_grav) const
{
    if(!IsActive()) return;

    std::memcpy(&p_grav, m_dynamicWorld->getGravity().m_floats, sizeof(glm::vec3));
}

void ROC::PhysicsManager::SetColliderScale(Collider *p_col, const glm::vec3 &p_scale)
{
    if(!IsActive()) return;

    btRigidBody *l_body = p_col->GetRigidBody();
    if(l_body)
    {
        m_dynamicWorld->removeRigidBody(l_body);
        p_col->SetScale(p_scale);
        m_dynamicWorld->addRigidBody(l_body);
    }
}

void ROC::PhysicsManager::UpdateWorldSteps(unsigned int p_fps)
{
    m_timeStep = (p_fps == 0U) ? g_physicsDefaultTimestep : (1.5f / static_cast<float>(p_fps));
}

void ROC::PhysicsManager::AddModel(Model *p_model)
{
    if(!IsActive()) return;

    if(p_model->GetSkeleton())
    {
        const Skeleton *l_skeleton = p_model->GetSkeleton();
        if(l_skeleton->HasStaticBoneCollision())
        {
            for(const auto l_skeletonCol : l_skeleton->GetCollision()) m_dynamicWorld->addRigidBody(l_skeletonCol->m_rigidBody);
        }
        if(l_skeleton->HasDynamicBoneCollision())
        {
            for(const auto l_joint : l_skeleton->GetJoints())
            {
                m_dynamicWorld->addRigidBody(l_joint->m_rootBody);
                for(const auto l_jointPart : l_joint->m_jointParts)
                {
                    m_dynamicWorld->addRigidBody(l_jointPart->m_rigidBody);
                    m_dynamicWorld->addConstraint(l_jointPart->m_constraint, true);
                }
            }
        }
    }
}

void ROC::PhysicsManager::RemoveModel(Model *p_model)
{
    if(!IsActive()) return;

    if(p_model->GetSkeleton())
    {
        Skeleton *l_skeleton = p_model->GetSkeleton();
        if(l_skeleton->HasStaticBoneCollision())
        {
            for(const auto l_skeletonCol : l_skeleton->GetCollision()) m_dynamicWorld->removeRigidBody(l_skeletonCol->m_rigidBody);
        }
        if(l_skeleton->HasDynamicBoneCollision())
        {
            for(const auto l_joint : l_skeleton->GetJoints())
            {
                m_dynamicWorld->removeRigidBody(l_joint->m_rootBody);
                for(const auto l_jointPart : l_joint->m_jointParts)
                {
                    m_dynamicWorld->removeRigidBody(l_jointPart->m_rigidBody);
                    m_dynamicWorld->removeConstraint(l_jointPart->m_constraint);
                }
            }
        }
    }
}

void ROC::PhysicsManager::AddCollision(Collider *p_col)
{
    if(!IsActive()) return;

    m_dynamicWorld->addRigidBody(p_col->GetRigidBody());
    m_collisions.push_back(p_col);
}

void ROC::PhysicsManager::RemoveCollision(Collider *p_col)
{
    if(!IsActive()) return;

    auto l_searchIter = std::find(m_collisions.begin(), m_collisions.end(), p_col);
    if(l_searchIter != m_collisions.end())
    {
        m_dynamicWorld->removeRigidBody(p_col->GetRigidBody());
        m_collisions.erase(l_searchIter);
    }
}

bool ROC::PhysicsManager::RayCast(const glm::vec3 &p_start, glm::vec3 &p_end, glm::vec3 &p_normal, Element *&p_element)
{
    if(!IsActive()) return false;

    bool l_result = false;
    if(p_start != p_end)
    {
        btVector3 l_start(p_start.x, p_start.y, p_start.z), l_end(p_end.x, p_end.y, p_end.z);
        btCollisionWorld::ClosestRayResultCallback l_rayResult(l_start, l_end);
        m_dynamicWorld->rayTest(l_start, l_end, l_rayResult);
        if(l_rayResult.hasHit())
        {
            void *l_colObject = l_rayResult.m_collisionObject->getUserPointer();
            if(l_colObject)
            {
                if(GetCore()->GetElementManager()->IsValidElement(reinterpret_cast<Element*>(l_colObject))) p_element = reinterpret_cast<Element*>(l_colObject);
            }
            std::memcpy(&p_end, l_rayResult.m_hitPointWorld.m_floats, sizeof(glm::vec3));
            std::memcpy(&p_normal, l_rayResult.m_hitNormalWorld.m_floats, sizeof(glm::vec3));
            l_result = true;
        }
    }
    return l_result;
}

void ROC::PhysicsManager::SetDebugDrawer(btIDebugDraw *p_drawer)
{
    if(!IsActive()) return;
    m_dynamicWorld->setDebugDrawer(p_drawer);
}

void ROC::PhysicsManager::DrawDebugWorld()
{
    if(!IsActive()) return;
    m_dynamicWorld->debugDrawWorld();
}

void ROC::PhysicsManager::DoPulse()
{
    if(!IsActive()) return;

    for(auto l_col : m_collisions) l_col->Update(ROC::Collider::CUS_Body);
    if(m_enabled) m_dynamicWorld->stepSimulation(m_timeStep, g_physicsDefaultSubstepsCount, g_physicsDefaultTimestep);
    for(auto l_col : m_collisions) l_col->Update(ROC::Collider::CUS_Matrix);
}

// ROC::IPhysicsManager
void ROC::PhysicsManager::SetIColliderScale(ICollider *p_col, const glm::vec3 &p_scale)
{
    SetColliderScale(dynamic_cast<Collider*>(p_col), p_scale);
}

bool ROC::PhysicsManager::RayCast(const glm::vec3 &p_start, glm::vec3 &p_end, glm::vec3 &p_normal, IElement *&p_element)
{
    ROC::Element *l_what = nullptr;
    bool l_result = RayCast(p_start, p_end, p_normal, l_what);
    p_element = (l_what ? dynamic_cast<ROC::IElement*>(l_what) : nullptr);
    return l_result;
}

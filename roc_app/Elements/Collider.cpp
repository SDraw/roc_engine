#include "stdafx.h"

#include "Elements/Collider.h"
#include "Elements/Model/Model.h"

namespace ROC
{

extern const glm::vec3 g_defaultScale;

}

ROC::Collider::Collider()
{
    m_elementType = ET_Collider;

    m_rigidBody = nullptr;
    m_motionType = CMT_Default;
}

ROC::Collider::~Collider()
{
    if(m_rigidBody)
    {
        delete m_rigidBody->getMotionState();
        delete m_rigidBody;
    }
}

bool ROC::Collider::Create(unsigned char p_type, const glm::vec3 &p_size, float p_mass)
{
    if(!m_rigidBody)
    {
        p_type = glm::clamp<unsigned char>(p_type, CT_Sphere, CT_Cone);

        btVector3 l_inertia;
        btCollisionShape *l_shape = nullptr;
        switch(p_type)
        {
            case CT_Sphere:
                l_shape = new btSphereShape(p_size.x);
                break;
            case CT_Box:
                l_shape = new btBoxShape(btVector3(p_size.x, p_size.y, p_size.z));
                break;
            case CT_Cylinder:
                l_shape = new btCylinderShape(btVector3(p_size.x, p_size.y, p_size.z));
                break;
            case CT_Capsule:
                l_shape = new btCapsuleShape(p_size.x, p_size.y);
                break;
            case CT_Cone:
                l_shape = new btConeShape(p_size.x, p_size.y);
                break;
        }
        if(l_shape)
        {
            l_shape->calculateLocalInertia(p_mass, l_inertia);
            btTransform l_transform;
            l_transform.setIdentity();
            btDefaultMotionState *l_fallMotionState = new btDefaultMotionState(l_transform);
            btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(p_mass, l_fallMotionState, l_shape, l_inertia);
            m_rigidBody = new btRigidBody(fallRigidBodyCI);
            m_rigidBody->setUserPointer(this);
        }
    }
    return (m_rigidBody != nullptr);
}

btRigidBody* ROC::Collider::GetRigidBody() const
{
    return m_rigidBody;
}

void ROC::Collider::SetScale(const glm::vec3 &p_val)
{
    if(m_rigidBody)
    {
        m_rigidBody->getCollisionShape()->setLocalScaling(btVector3(p_val.x, p_val.y, p_val.z));
        Transformable::SetScale(p_val);
    }
}

void ROC::Collider::SetVelocity(const glm::vec3 &p_val)
{
    if(m_rigidBody)
    {
        m_rigidBody->setLinearVelocity(btVector3(p_val.x, p_val.y, p_val.z));
        m_rigidBody->activate(true);
    }
}

void ROC::Collider::GetVelocity(glm::vec3 &p_val) const
{
    if(m_rigidBody) std::memcpy(&p_val, m_rigidBody->getLinearVelocity().m_floats, sizeof(glm::vec3));
}

void ROC::Collider::SetAngularVelocity(const glm::vec3 &p_val)
{
    if(m_rigidBody)
    {
        m_rigidBody->setAngularVelocity(btVector3(p_val.x, p_val.y, p_val.z));
        m_rigidBody->activate(true);
    }
}

void ROC::Collider::GetAngularVelocity(glm::vec3 &p_val) const
{
    if(m_rigidBody) std::memcpy(&p_val, m_rigidBody->getAngularVelocity().m_floats, sizeof(glm::vec3));
}

void ROC::Collider::SetLinearFactor(const glm::vec3 &p_val)
{
    if(m_rigidBody) m_rigidBody->setLinearFactor(btVector3(p_val.x, p_val.y, p_val.z));
}

void ROC::Collider::GetLinearFactor(glm::vec3 &p_val) const
{
    if(m_rigidBody) std::memcpy(&p_val, m_rigidBody->getLinearFactor().m_floats, sizeof(glm::vec3));
}

void ROC::Collider::SetAngularFactor(const glm::vec3 &p_val)
{
    if(m_rigidBody) m_rigidBody->setAngularFactor(btVector3(p_val.x, p_val.y, p_val.z));
}

void ROC::Collider::GetAngularFactor(glm::vec3 &p_val) const
{
    if(m_rigidBody) std::memcpy(&p_val, m_rigidBody->getAngularFactor().m_floats, sizeof(glm::vec3));
}

float ROC::Collider::GetMass() const
{
    float l_invMass = m_rigidBody ? m_rigidBody->getInvMass() : -1.f;
    float l_mass = ((l_invMass == 0.f) ? 0.f : (1.f / l_invMass));
    return l_mass;
}

void ROC::Collider::SetFriction(float p_val)
{
    if(m_rigidBody)
    {
        m_rigidBody->setFriction(p_val);
        m_rigidBody->activate(true);
    }
}

float ROC::Collider::GetFriction() const
{
    float l_result = -1.f;
    if(m_rigidBody) l_result = m_rigidBody->getFriction();
    return l_result;
}

void ROC::Collider::SetRestitution(float p_val)
{
    if(m_rigidBody) m_rigidBody->setRestitution(p_val);
}

float ROC::Collider::GetRestitution() const
{
    return (m_rigidBody ? m_rigidBody->getRestitution() : -1.f);
}

void ROC::Collider::ApplyForce(const glm::vec3 &p_force, const glm::vec3 &p_rp)
{
    if(m_rigidBody)
    {
        btVector3 l_force(p_force.x, p_force.y, p_force.z);
        btVector3 l_relPos(p_rp.x, p_rp.y, p_rp.z);
        m_rigidBody->applyForce(l_force, l_relPos);
    }
}

void ROC::Collider::ApplyCentralForce(const glm::vec3 &p_force)
{
    if(m_rigidBody) m_rigidBody->applyCentralForce(btVector3(p_force.x, p_force.y, p_force.z));
}

void ROC::Collider::ApplyImpulse(const glm::vec3 &p_impulse, const glm::vec3 &p_rp)
{
    if(m_rigidBody)
    {
        btVector3 l_impulse(p_impulse.x, p_impulse.y, p_impulse.z);
        btVector3 l_relPos(p_rp.x, p_rp.y, p_rp.z);
        m_rigidBody->applyImpulse(l_impulse, l_relPos);
    }
}

void ROC::Collider::ApplyCentralImpulse(const glm::vec3 &p_impulse)
{
    if(m_rigidBody) m_rigidBody->applyCentralImpulse(btVector3(p_impulse.x, p_impulse.y, p_impulse.z));
}

void ROC::Collider::ApplyTorque(const glm::vec3 &p_torque, bool p_impulse)
{
    if(m_rigidBody)
    {
        btVector3 l_torque(p_torque.x, p_torque.y, p_torque.z);
        p_impulse ? m_rigidBody->applyTorqueImpulse(l_torque) : m_rigidBody->applyTorque(l_torque);
    }
}

void ROC::Collider::SetMotionType(unsigned char p_type)
{
    if(m_rigidBody)
    {
        m_motionType = glm::clamp<unsigned char>(p_type, CMT_Default, CMT_Kinematic);
        switch(m_motionType)
        {
            case CMT_Default:
            {
                m_rigidBody->setCollisionFlags(0);
                m_rigidBody->setActivationState(ACTIVE_TAG);
            } break;
            case CMT_Static:
            {
                m_rigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
                m_rigidBody->setActivationState(ACTIVE_TAG);
            } break;
            case CMT_Kinematic:
            {
                m_rigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
                m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
            } break;
        }
    }
}

unsigned char ROC::Collider::GetMotionType() const
{
    return m_motionType;
}

bool ROC::Collider::IsActive() const
{
    return (m_rigidBody ? m_rigidBody->isActive() : false);
}

void ROC::Collider::Update(CollisionUpdateStage p_stage)
{
    if(m_rigidBody)
    {
        switch(p_stage)
        {
            case CUS_Body:
            {
                if(NeedsUpdate())
                {
                    btTransform l_transform = btTransform::getIdentity();

                    const glm::vec3 &l_pos = GetPosition();
                    l_transform.setOrigin(btVector3(l_pos.x, l_pos.y, l_pos.z));

                    const glm::quat &l_rot = GetRotation();
                    l_transform.setRotation(btQuaternion(l_rot.x, l_rot.y, l_rot.z, l_rot.w));

                    switch(m_motionType)
                    {
                        case CMT_Default: case CMT_Static:
                            m_rigidBody->setCenterOfMassTransform(l_transform);
                            break;
                        case CMT_Kinematic:
                        {
                            m_rigidBody->getMotionState()->setWorldTransform(l_transform);
                        } break;
                    }
                }
            } break;

            case CUS_Matrix:
            {
                if(m_motionType == CMT_Default)
                {
                    const btTransform &l_transform = m_rigidBody->getCenterOfMassTransform();
                    const btVector3 &l_pos = l_transform.getOrigin();
                    const glm::vec3 l_position(l_pos.m_floats[0], l_pos.m_floats[1], l_pos.m_floats[2]);
                    const btQuaternion l_rot = l_transform.getRotation();
                    const glm::quat l_rotation(l_rot.w(), l_rot.x(), l_rot.y(), l_rot.z());
                    SetPosition(l_position);
                    SetRotation(l_rotation);
                }

                Transformable::UpdateMatrix();
            } break;
        }
    }
}

// ROC::Collidable
void ROC::Collider::GetRigidBodies(std::vector<btRigidBody*> &p_vec)
{
    if(m_rigidBody) p_vec.push_back(m_rigidBody);
}

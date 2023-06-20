#include "stdafx.h"

#include "Elements/Model/Model.h"
#include "Elements/Animation/Animation.h"
#include "Elements/Mesh/Mesh.h"
#include "Elements/Model/Animator.h"
#include "Elements/Model/Bone.h"
#include "Elements/Model/Skeleton.h"

namespace ROC
{

extern const glm::mat4 g_identityMatrix;

}

ROC::Model::Model(Mesh *p_mesh)
{
    m_elementType = ET_Model;

    m_boundSphereRaduis = 0.f;
    m_visibility = false;
    m_visibilityDistance = 0.f;

    m_mesh = p_mesh;

    m_animator = nullptr;
    m_skeleton = nullptr;
    if(m_mesh)
    {
        m_boundSphereRaduis = m_mesh->GetBoundSphereRadius();
        if(m_mesh->HasBonesData())
        {
            m_skeleton = new Skeleton(m_mesh->GetBonesData());
            if(m_mesh->HasBonesCollisionData()) m_skeleton->InitStaticBoneCollision(m_mesh->GetBonesCollisionData(), this);
            if(m_mesh->HasJointsData()) m_skeleton->InitDynamicBoneCollision(m_mesh->GetJointsData(), this);
            m_animator = new Animator();
        }

        Element::Link(m_mesh, this);
    }
}

ROC::Model::~Model()
{
    delete m_animator;
    delete m_skeleton;
}

ROC::Mesh* ROC::Model::GetMesh() const
{
    return m_mesh;
}

float ROC::Model::GetBoundSphereRadius() const
{
    return m_boundSphereRaduis;
}

bool ROC::Model::SetAnimation(Animation *p_anim)
{
    bool l_result = false;
    if(m_skeleton && m_animator && (m_skeleton->GetBonesCount() == p_anim->GetBonesCount()) && (m_animator->GetAnimation() != p_anim))
    {
        Animation *l_oldAnim = m_animator->GetAnimation();
        if(l_oldAnim) Element::Unlink(l_oldAnim, this);

        Element::Link(p_anim, this);
        m_animator->SetAnimation(p_anim);

        l_result = true;
    }
    return l_result;
}

bool ROC::Model::RemoveAnimation()
{
    bool l_result = false;
    if(m_animator)
    {
        Animation *l_anim = m_animator->GetAnimation();
        if(l_anim)
        {
            Element::Unlink(l_anim, this);
            m_animator->SetAnimation(nullptr);

            l_result = true;
        }
    }
    return l_result;
}

ROC::Animation* ROC::Model::GetAnimation() const
{
    Animation *l_anim = nullptr;
    if(m_animator) l_anim = m_animator->GetAnimation();
    return l_anim;
}

bool ROC::Model::PlayAnimation(bool p_loop)
{
    bool l_result = false;
    if(m_animator) l_result = m_animator->Play(p_loop);
    return l_result;
}

bool ROC::Model::PauseAnimation()
{
    bool l_result = false;
    if(m_animator) l_result = m_animator->Pause();
    return l_result;
}

bool ROC::Model::ResetAnimation()
{
    bool l_result = false;
    if(m_animator) l_result = m_animator->Reset();
    return l_result;
}

bool ROC::Model::GetAnimationProperty(ModelAnimationProperty p_prop, float &p_value)
{
    if(m_animator)
    {
        switch(p_prop)
        {
            case MAP_Speed:
                p_value = m_animator->GetSpeed();
                break;
            case MAP_Progress:
                p_value = m_animator->GetProgress();
                break;
            case Map_BlendTime:
                p_value = static_cast<float>(m_animator->GetBlendTime());
                break;
        }
    }
    return (m_animator != nullptr);
}

bool ROC::Model::SetAnimationProperty(ModelAnimationProperty p_prop, float p_value)
{
    if(m_animator)
    {
        switch(p_prop)
        {
            case MAP_Speed:
                m_animator->SetSpeed(p_value);
                break;
            case MAP_Progress:
                m_animator->SetProgress(p_value);
                break;
            case Map_BlendTime:
                m_animator->SetBlendTime(static_cast<unsigned int>(p_value));
                break;
        }
    }
    return (m_animator != nullptr);
}

ROC::Skeleton* ROC::Model::GetSkeleton() const
{
    return m_skeleton;
}

void ROC::Model::SetVisibility(bool p_state)
{
    m_visibility = p_state;

}

bool ROC::Model::IsVisible() const
{
    return m_visibility;
}

void ROC::Model::SetVisibilityDistance(float p_dist)
{
    m_visibilityDistance = p_dist;
}

float ROC::Model::GetVisibilityDistance() const
{
    return m_visibilityDistance;
}

void ROC::Model::Update(ModelUpdateStage p_stage)
{
    switch(p_stage)
    {
        case MUS_Matrix:
        {
            // Matrix
            UpdateMatrix();

            // Bounding sphere
            if(IsUpdated() && m_mesh)
            {
                m_boundSphereRaduis = m_mesh->GetBoundSphereRadius();
                if(IsScaled()) m_boundSphereRaduis *= glm::length(GetScale());
            }
        } break;

        case MUS_SkeletonStatic:
        {
            // Animaton
            if(m_skeleton)
            {
                if(m_animator)
                {
                    Animation *l_anim = m_animator->GetAnimation();
                    if(l_anim)
                    {
                        m_animator->Update();
                        if(m_animator->IsPlaying()) m_skeleton->Update(l_anim, m_animator->GetTick(), m_animator->GetBlendValue());
                    }
                }

                // Static collision
                m_skeleton->UpdateCollision(Skeleton::SUS_Static, GetMatrix());
            }
        } break;

        case MUS_SkeletonDynamic:
        {
            // Dynamic collision
            if(m_skeleton) m_skeleton->UpdateCollision(Skeleton::SUS_Dynamic, GetMatrix());
        } break;
    }
}

void ROC::Model::OnParentRemoved(Element *p_parent)
{
    switch(p_parent->GetElementType())
    {
        case ET_Animation:
            m_animator->SetAnimation(nullptr);
            break;
        case ET_Mesh:
            m_mesh = nullptr;
            break;
    }

    Element::OnParentRemoved(p_parent);
}

// ROC::Collidable
void ROC::Model::GetRigidBodies(std::vector<btRigidBody*> &p_vec)
{
    if(m_skeleton)
    {
        for(auto l_skeletonCol : m_skeleton->GetCollision()) p_vec.push_back(l_skeletonCol->m_rigidBody);
        for(auto l_joint : m_skeleton->GetJoints())
        {
            p_vec.push_back(l_joint->m_rootBody);
            for(auto l_jointPart : l_joint->m_jointParts) p_vec.push_back(l_jointPart->m_rigidBody);
        }
    }
}

// ROC::IModel
ROC::IMesh* ROC::Model::GetIMesh() const
{
    return m_mesh;
}

bool ROC::Model::SetIAnimation(IAnimation *p_anim)
{
    return SetAnimation(dynamic_cast<Animation*>(p_anim));
}

ROC::IAnimation* ROC::Model::GetIAnimation() const
{
    return GetAnimation();
}

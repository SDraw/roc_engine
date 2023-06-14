#include "stdafx.h"

#include "Elements/Model/Bone.h"
#include "Elements/Animation/BoneFrameData.h"
#include "Utils/Transformation.h"

namespace ROC
{

extern const glm::mat4 g_identityMatrix;

}

ROC::Bone::Bone(const std::string &p_name, const glm::quat &p_rot, const glm::vec3 &p_pos, const glm::vec3 &p_scl)
{
    m_parent = nullptr;
#ifdef _DEBUG
    m_name.assign(p_name);
#endif

    m_localTransform = new Transformation();
    m_localTransform->SetPosition(p_pos);
    m_localTransform->SetRotation(p_rot);
    m_localTransform->SetScale(p_scl);
    m_localTransform->UpdateMatrix();

    m_fullMatrix = g_identityMatrix;
    m_bindMatrix = g_identityMatrix;
    m_poseMatrix = g_identityMatrix;
    m_updated = true;
}

ROC::Bone::~Bone()
{
    m_children.clear();
    delete m_localTransform;
    m_parent = nullptr;
}

void ROC::Bone::GenerateBindPose()
{
    if(m_parent) m_fullMatrix = m_parent->m_fullMatrix*m_localTransform->GetMatrix();
    else std::memcpy(&m_fullMatrix, &m_localTransform->GetMatrix(), sizeof(glm::mat4));
    m_bindMatrix = glm::inverse(m_fullMatrix);
}

void ROC::Bone::SetParent(Bone *p_bone)
{
    m_parent = p_bone;
}

ROC::Bone* ROC::Bone::GetParent() const
{
    return m_parent;
}

void ROC::Bone::AddChild(ROC::Bone *p_bone)
{
    m_children.push_back(p_bone);
}

const std::vector<ROC::Bone*>& ROC::Bone::GetChildren() const
{
    return m_children;
}

const ROC::Transformation* ROC::Bone::GetLocalTransformation() const
{
    return m_localTransform;
}

const glm::mat4& ROC::Bone::GetBindMatrix() const
{
    return m_bindMatrix;
}

const glm::mat4& ROC::Bone::GetFullMatrix() const
{
    return m_fullMatrix;
}

const glm::mat4& ROC::Bone::GetPoseMatrix() const
{
    return m_poseMatrix;
}

void ROC::Bone::SetFrameData(BoneFrameData *p_data)
{
    m_localTransform->SetPosition(p_data->m_position);
    m_localTransform->SetRotation(p_data->m_rotation);
    m_localTransform->SetScale(p_data->m_scale);
}

void ROC::Bone::SetFrameData(BoneFrameData *p_data, float p_blend)
{
    const glm::vec3 l_pos = glm::mix(m_localTransform->GetPosition(), p_data->m_position, p_blend);
    m_localTransform->SetPosition(l_pos);

    const glm::quat l_rot = glm::slerp(m_localTransform->GetRotation(), p_data->m_rotation, p_blend);
    m_localTransform->SetRotation(l_rot);

    const glm::vec3 l_scl = glm::mix(m_localTransform->GetScale(), p_data->m_scale, p_blend);
    m_localTransform->SetScale(l_scl);
}

void ROC::Bone::SetFullMatrix(const btTransform &p_transform)
{
    p_transform.getOpenGLMatrix(glm::value_ptr(m_fullMatrix));
    m_updated = true;
}

void ROC::Bone::SetPoseMatrix(const btTransform &p_transform)
{
    p_transform.getOpenGLMatrix(glm::value_ptr(m_poseMatrix));
    m_updated = true;
}

bool ROC::Bone::IsUpdated() const
{
    return m_updated;
}

void ROC::Bone::Update()
{
    m_updated = false;
    m_localTransform->UpdateMatrix();
    if(m_parent)
    {
        if(m_parent->IsUpdated() || m_localTransform->IsUpdated())
        {
            m_fullMatrix = m_parent->m_fullMatrix*m_localTransform->GetMatrix();
            m_updated = true;
        }
    }
    else
    {
        if(m_localTransform->IsUpdated())
        {
            std::memcpy(&m_fullMatrix, &m_localTransform->GetMatrix(), sizeof(glm::mat4));
            m_updated = true;
        }
    }
    if(m_updated) m_poseMatrix = m_fullMatrix*m_bindMatrix;
}

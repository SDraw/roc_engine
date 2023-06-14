#include "stdafx.h"
#include "Utils/Transformation.h"

namespace ROC
{

extern const glm::vec3 g_emptyVec3;
extern const glm::vec3 g_defaultScale;
extern const glm::quat g_defaultRotation;
extern const glm::mat4 g_identityMatrix;
extern const float g_epsilon;

}

ROC::Transformation::Transformation()
{
    m_position = g_emptyVec3;
    m_rotation = g_defaultRotation;
    m_scale = g_defaultScale;
    m_matrix = g_identityMatrix;
    m_useScale = false;
    m_update = false;
    m_updated = false;
}

void ROC::Transformation::SetPosition(const glm::vec3 &p_pos)
{
    if(m_position != p_pos)
    {
        std::memcpy(&m_position, &p_pos, sizeof(glm::vec3));
        m_update = true;
    }
}

const glm::vec3& ROC::Transformation::GetPosition() const
{
    return m_position;
}

void ROC::Transformation::SetRotation(const glm::quat &p_rot)
{
    if(m_rotation != p_rot)
    {
        std::memcpy(&m_rotation, &p_rot, sizeof(glm::quat));
        m_update = true;
    }
}

const glm::quat& ROC::Transformation::GetRotation() const
{
    return m_rotation;
}

void ROC::Transformation::SetScale(const glm::vec3 &p_scl)
{
    if(m_scale != p_scl)
    {
        std::memcpy(&m_scale, &p_scl, sizeof(glm::vec3));
        for(int i = 0; i < 3; i++)
        {
            if(glm::epsilonNotEqual(m_scale[i], g_defaultScale[i], g_epsilon))
            {
                m_useScale = true;
                break;
            }
        }
        m_update = true;
    }
}

const glm::vec3& ROC::Transformation::GetScale() const
{
    return m_scale;
}

bool ROC::Transformation::IsScaled() const
{
    return m_useScale;
}

bool ROC::Transformation::NeedsUpdate() const
{
    return m_update;
}

bool ROC::Transformation::IsUpdated() const
{
    return m_updated;
}

void ROC::Transformation::GetMatrix(glm::mat4 &p_mat) const
{
    std::memcpy(&p_mat, &m_matrix, sizeof(glm::mat4));
}

const glm::mat4& ROC::Transformation::GetMatrix() const
{
    return m_matrix;
}

void ROC::Transformation::UpdateMatrix()
{
    m_updated = false;
    if(m_update)
    {
        btTransform l_transform = btTransform::getIdentity();
        l_transform.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));
        l_transform.setRotation(btQuaternion(m_rotation.x, m_rotation.y, m_rotation.z, m_rotation.w));
        l_transform.getOpenGLMatrix(glm::value_ptr(m_matrix));
        if(m_useScale) m_matrix *= glm::scale(g_identityMatrix, m_scale);
        m_update = false;
        m_updated = true;
    }
}

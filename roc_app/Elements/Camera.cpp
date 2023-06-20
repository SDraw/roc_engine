#include "stdafx.h"

#include "Elements/Camera.h"

#include "Utils/MathUtils.h"

namespace ROC
{

extern const glm::vec3 g_emptyVec3;
extern const glm::vec3 g_defaultDirection;
const glm::vec3 g_cameraDefaultUpDirection(0.f, 1.f, 0.f);

}

ROC::Camera::Camera(unsigned char p_type)
{
    m_elementType = ET_Camera;

    m_type = glm::clamp<unsigned char>(p_type, CPT_Perspective, CPT_Screen);

    m_viewDirection = g_defaultDirection;
    m_upDirection = g_cameraDefaultUpDirection;
    m_viewMatrix = glm::lookAt(g_emptyVec3, m_viewDirection, g_cameraDefaultUpDirection);

    m_fov = glm::pi<float>() / 4.0f;
    m_aspectRatio = 640.f / 480.f;
    m_orthoParams = glm::vec4(-1.f, 1.f, -1.f, 1.f);
    m_depth = glm::vec2(1.f, 10.f);
    m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_depth.x, m_depth.y);
    m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;

    m_rebuildView = false;
    m_rebuildProjection = false;
}

void ROC::Camera::SetProjectionType(unsigned char p_type)
{
    if(m_type != p_type)
    {
        m_type = glm::clamp<unsigned char>(p_type, CPT_Perspective, CPT_Screen);
        m_rebuildProjection = true;
    }
}

unsigned char ROC::Camera::GetProjectionType() const
{
    return m_type;
}

void ROC::Camera::SetPosition(const glm::vec3& p_pos)
{
    if(Transformable::GetPosition() != p_pos)
    {
        Transformable::SetPosition(p_pos);
        m_rebuildView = true;
    }

    Transformable::SetPosition(p_pos);
}

void ROC::Camera::SetDirection(const glm::vec3& p_dir)
{
    if(m_viewDirection != p_dir)
    {
        std::memcpy(&m_viewDirection, &p_dir, sizeof(glm::vec3));
        m_rebuildView = true;
    }
}

void ROC::Camera::SetRotation(const glm::quat &p_rot)
{
    glm::vec3 l_dir = p_rot * g_defaultDirection;
    if(m_viewDirection != l_dir)
    {
        std::memcpy(&m_viewDirection, &l_dir, sizeof(glm::vec3));
        m_rebuildView = true;
    }

    glm::vec3 l_upDir = p_rot * g_cameraDefaultUpDirection;
    if(m_upDirection != l_upDir)
    {
        std::memcpy(&m_upDirection, &l_upDir, sizeof(glm::vec3));
        m_rebuildView = true;
    }

    Transformable::SetRotation(p_rot);
}

const glm::vec3& ROC::Camera::GetDirection() const
{
    return m_viewDirection;
}

void ROC::Camera::SetUpDirection(const glm::vec3 &p_dir)
{
    if(m_upDirection != p_dir)
    {
        std::memcpy(&m_upDirection, &p_dir, sizeof(glm::vec3));
        m_rebuildView = true;
    }
}

const glm::vec3& ROC::Camera::GetUpDirection() const
{
    return m_upDirection;
}

void ROC::Camera::SetFOV(float p_fov)
{
    if(m_fov != p_fov)
    {
        m_fov = p_fov;
        m_rebuildProjection = true;
    }
}

float ROC::Camera::GetFOV() const
{
    return m_fov;
}

void ROC::Camera::SetAspectRatio(float p_ratio)
{
    if(m_aspectRatio != p_ratio)
    {
        m_aspectRatio = p_ratio;
        m_rebuildProjection = true;
    }
}

float ROC::Camera::GetAspectRatio() const
{
    return m_aspectRatio;
}

void ROC::Camera::SetOrthoParams(const glm::vec4 &p_size)
{
    if(m_orthoParams != p_size)
    {
        std::memcpy(&m_orthoParams, &p_size, sizeof(glm::vec4));
        m_rebuildProjection = true;
    }
}

const glm::vec4& ROC::Camera::GetOrthoParams() const
{
    return m_orthoParams;
}

void ROC::Camera::SetDepth(const glm::vec2 &p_depth)
{
    if(m_depth != p_depth)
    {
        std::memcpy(&m_depth, &p_depth, sizeof(glm::vec2));
        m_rebuildProjection = true;
    }
}

const glm::vec2& ROC::Camera::GetDepth() const
{
    return m_depth;
}

const glm::mat4& ROC::Camera::GetViewMatrix() const
{
    return m_viewMatrix;
}

const glm::mat4& ROC::Camera::GetProjectionMatrix() const
{
    return m_projectionMatrix;
}

const glm::mat4& ROC::Camera::GetViewProjectionMatrix() const
{
    return m_viewProjectionMatrix;
}

void ROC::Camera::Update()
{
    Transformable::UpdateMatrix();

    if(m_rebuildView)
    {
        glm::vec3 l_viewPoint = Transformable::GetPosition() + m_viewDirection;
        m_viewMatrix = glm::lookAt(Transformable::GetPosition(), l_viewPoint, m_upDirection);
    }
    if(m_rebuildProjection)
    {
        switch(m_type)
        {
            case CPT_Perspective:
                m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_depth.x, m_depth.y);
                break;
            case CPT_Orthogonal:
                m_projectionMatrix = glm::ortho(m_orthoParams.x, m_orthoParams.y, m_orthoParams.z, m_orthoParams.w, m_depth.x, m_depth.y);
                break;
            case CPT_Screen:
                m_projectionMatrix = glm::ortho(m_orthoParams.x, m_orthoParams.y, m_orthoParams.z, m_orthoParams.w);
                break;
        }
    }
    if(m_rebuildView || m_rebuildProjection)
    {
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        m_planes[0] = glm::row(m_viewProjectionMatrix, 3) + glm::row(m_viewProjectionMatrix, 0);
        m_planes[1] = glm::row(m_viewProjectionMatrix, 3) - glm::row(m_viewProjectionMatrix, 0);
        m_planes[2] = glm::row(m_viewProjectionMatrix, 3) + glm::row(m_viewProjectionMatrix, 1);
        m_planes[3] = glm::row(m_viewProjectionMatrix, 3) - glm::row(m_viewProjectionMatrix, 1);
        m_planes[4] = glm::row(m_viewProjectionMatrix, 3) + glm::row(m_viewProjectionMatrix, 2);
        m_planes[5] = glm::row(m_viewProjectionMatrix, 3) - glm::row(m_viewProjectionMatrix, 2);
        for(auto &l_plane : m_planes) l_plane /= sqrtf(l_plane.x*l_plane.x + l_plane.y*l_plane.y + l_plane.z*l_plane.z);

        m_rebuildView = false;
        m_rebuildProjection = false;
    }
}

bool ROC::Camera::IsInFrustum(const glm::vec3 &p_pos, float p_radius) const
{
    bool l_result = true;
    for(auto const &l_plane : m_planes)
    {
        if(l_plane.x*p_pos.x + l_plane.y*p_pos.y + l_plane.z*p_pos.z + l_plane.w < -p_radius)
        {
            l_result = false;
            break;
        }
    }
    return l_result;
}

bool ROC::Camera::IsInFrustum(const glm::mat4 &p_mat, float p_radius) const
{
    bool l_result = true;
    btTransform l_transform = btTransform::getIdentity();
    l_transform.setFromOpenGLMatrix(glm::value_ptr(p_mat));
    const btVector3 &l_position = l_transform.getOrigin();
    for(auto const &l_plane : m_planes)
    {
        if(l_plane.x*l_position.x() + l_plane.y*l_position.y() + l_plane.z*l_position.z() + l_plane.w < -p_radius)
        {
            l_result = false;
            break;
        }
    }
    return l_result;
}

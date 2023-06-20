#pragma once
#include "Interfaces/ICamera.h"
#include "Elements/Element.h"
#include "Elements/Transformable.h"

namespace ROC
{

class Camera final : public Element, public Transformable, public ICamera
{
    unsigned char m_type;

    glm::vec3 m_viewDirection;
    glm::vec3 m_upDirection;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewProjectionMatrix;
    bool m_rebuildView;
    bool m_rebuildProjection;

    float m_fov;
    float m_aspectRatio;
    glm::vec4 m_orthoParams;
    glm::vec2 m_depth;

    std::array<glm::vec4, 6U> m_planes;

    Camera(const Camera &that) = delete;
    Camera& operator=(const Camera &that) = delete;
public:
    explicit Camera(unsigned char p_type);
    ~Camera() = default;

    void SetProjectionType(unsigned char p_type);
    unsigned char GetProjectionType() const;

    void SetFOV(float p_fov);
    float GetFOV() const;

    void SetAspectRatio(float p_ratio);
    float GetAspectRatio() const;

    void SetOrthoParams(const glm::vec4 &p_size);
    const glm::vec4& GetOrthoParams() const;

    void SetDepth(const glm::vec2 &p_depth);
    const glm::vec2& GetDepth() const;

    void SetDirection(const glm::vec3 &p_dir);
    const glm::vec3& GetDirection() const;

    void SetUpDirection(const glm::vec3 &p_dir);
    const glm::vec3& GetUpDirection() const;


    const glm::mat4& GetViewMatrix() const;
    const glm::mat4& GetProjectionMatrix() const;
    const glm::mat4& GetViewProjectionMatrix() const;

    bool IsInFrustum(const glm::vec3 &p_pos, float p_radius) const;
    bool IsInFrustum(const glm::mat4 &p_mat, float p_radius) const;

    void Update();

    // ROC::Transformable
    void SetPosition(const glm::vec3 &p_pos) override;
    void SetRotation(const glm::quat &p_dir) override;
};

}

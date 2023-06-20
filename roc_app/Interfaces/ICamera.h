#pragma once

namespace ROC
{

class ICamera
{
protected:
    ~ICamera() = default;
public:
    enum CameraProjectionType : unsigned char
    {
        CPT_Perspective = 0U,
        CPT_Orthogonal,
        CPT_Screen
    };

    virtual void SetProjectionType(unsigned char p_type) = 0;
    virtual unsigned char GetProjectionType() const = 0;
    virtual void SetFOV(float p_fov) = 0;
    virtual float GetFOV() const = 0;
    virtual void SetAspectRatio(float p_ratio) = 0;
    virtual float GetAspectRatio() const = 0;
    virtual void SetOrthoParams(const glm::vec4 &p_size) = 0;
    virtual const glm::vec4& GetOrthoParams() const = 0;
    virtual void SetDepth(const glm::vec2 &p_depth) = 0;
    virtual const glm::vec2& GetDepth() const = 0;
    virtual void SetDirection(const glm::vec3 &p_dir) = 0;
    virtual const glm::vec3& GetDirection() const = 0;
    virtual void SetUpDirection(const glm::vec3 &p_dir) = 0;
    virtual const glm::vec3& GetUpDirection() const = 0;
    virtual const glm::mat4& GetViewMatrix() const = 0;
    virtual const glm::mat4& GetProjectionMatrix() const = 0;
    virtual const glm::mat4& GetViewProjectionMatrix() const = 0;
};

}

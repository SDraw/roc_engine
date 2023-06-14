#pragma once

namespace ROC
{

class ITransformable
{
public:
    virtual void SetPosition(const glm::vec3 &p_pos) = 0;
    virtual const glm::vec3& GetPosition() const = 0;
    virtual void SetRotation(const glm::quat &p_rot) = 0;
    virtual const glm::quat& GetRotation() const = 0;
    virtual void SetScale(const glm::vec3 &p_scl) = 0;
    virtual const glm::vec3& GetScale() const = 0;
    virtual const glm::mat4& GetMatrix() const = 0;
};

}

#pragma once
#include "Interfaces/ITransformable.h"
#include "Utils/Transformation.h"

namespace ROC
{

class Transformation;

class Transformable : public ITransformable
{
    Transformation *m_transform;
protected:
    Transformable();
    virtual ~Transformable();
public:
    // ROC::ITransformable
    void SetPosition(const glm::vec3 &p_pos);
    const glm::vec3& GetPosition() const;
    void SetRotation(const glm::quat &p_rot);
    const glm::quat& GetRotation() const;
    void SetScale(const glm::vec3 &p_scl);
    const glm::vec3& GetScale() const;
    const glm::mat4& GetMatrix() const;

    bool NeedsUpdate() const;
    void UpdateMatrix();
    bool IsUpdated() const;
    bool IsScaled() const;
};

}

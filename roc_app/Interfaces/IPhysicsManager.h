#pragma once

namespace ROC
{

class IElement;
class ICollision;
class IModel;

class IPhysicsManager
{
public:
    virtual void SetPhysicsEnabled(bool p_value) = 0;
    virtual bool GetPhysicsEnabled() const = 0;
    virtual void SetFloorEnabled(bool p_value) = 0;
    virtual bool GetFloorEnabled() const = 0;
    virtual void SetGravity(const glm::vec3 &p_grav) = 0;
    virtual void GetGravity(glm::vec3 &p_grav) const = 0;
    virtual void SetCollisionScale(ICollision *p_col, const glm::vec3 &p_scale) = 0;
    virtual bool RayCast(const glm::vec3 &p_start, glm::vec3 &p_end, glm::vec3 &p_normal, IElement *&p_element) = 0;
};

}

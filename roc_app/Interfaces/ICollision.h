#pragma once
#include "IElement.h"

namespace ROC
{

class ICollision
{
public:
    enum CollisionType : unsigned char
    {
        CT_Sphere = 0U,
        CT_Box,
        CT_Cylinder,
        CT_Capsule,
        CT_Cone
    };
    enum CollisionMotionType : unsigned char
    {
        CMT_Default = 0U,
        CMT_Static,
        CMT_Kinematic
    };

    virtual void SetVelocity(const glm::vec3 &p_val) = 0;
    virtual void GetVelocity(glm::vec3 &p_val) const = 0;
    virtual void SetAngularVelocity(const glm::vec3 &p_val) = 0;
    virtual void GetAngularVelocity(glm::vec3 &p_val) const = 0;
    virtual void SetLinearFactor(const glm::vec3 &p_val) = 0;
    virtual void GetLinearFactor(glm::vec3 &p_val) const = 0;
    virtual void SetAngularFactor(const glm::vec3 &p_val) = 0;
    virtual void GetAngularFactor(glm::vec3 &p_val) const = 0;
    virtual float GetMass() const = 0;
    virtual void SetFriction(float p_val) = 0;
    virtual float GetFriction() const = 0;
    virtual void SetRestitution(float p_val) = 0;
    virtual float GetRestitution() const = 0;
    virtual void ApplyForce(const glm::vec3 &p_force, const glm::vec3 &p_rp) = 0;
    virtual void ApplyCentralForce(const glm::vec3 &p_force) = 0;
    virtual void ApplyImpulse(const glm::vec3 &p_impulse, const glm::vec3 &p_rp) = 0;
    virtual void ApplyCentralImpulse(const glm::vec3 &p_impulse) = 0;
    virtual void ApplyTorque(const glm::vec3 &p_torque, bool p_impulse) = 0;
    virtual void SetMotionType(unsigned char p_type) = 0;
    virtual unsigned char GetMotionType() const = 0;
};

}

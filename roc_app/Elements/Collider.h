#pragma once
#include "Interfaces/ICollider.h"
#include "Elements/Collidable.h"
#include "Elements/Element.h"
#include "Elements/Transformable.h"

namespace ROC
{

class Collider final : public Element, public Collidable, public Transformable, public ICollider
{
    btRigidBody *m_rigidBody;
    unsigned char m_motionType;

    Collider(const Collider &that) = delete;
    Collider& operator=(const Collider &that) = delete;

    // ROC::Collidable
    void GetRigidBodies(std::vector<btRigidBody*> &p_vec);
public:
    enum CollisionUpdateStage : unsigned char
    {
        CUS_Body,
        CUS_Matrix
    };

    Collider();
    ~Collider();

    bool Create(unsigned char p_type, const glm::vec3 &p_size, float p_mass);

    void SetVelocity(const glm::vec3 &p_val);
    void GetVelocity(glm::vec3 &p_val) const;

    void SetAngularVelocity(const glm::vec3 &p_val);
    void GetAngularVelocity(glm::vec3 &p_val) const;

    void SetLinearFactor(const glm::vec3 &p_val);
    void GetLinearFactor(glm::vec3 &p_val) const;

    void SetAngularFactor(const glm::vec3 &p_val);
    void GetAngularFactor(glm::vec3 &p_val) const;

    float GetMass() const;

    void SetFriction(float p_val);
    float GetFriction() const;

    void SetRestitution(float p_val);
    float GetRestitution() const;

    void ApplyForce(const glm::vec3 &p_force, const glm::vec3 &p_rp);
    void ApplyCentralForce(const glm::vec3 &p_force);

    void ApplyImpulse(const glm::vec3 &p_impulse, const glm::vec3 &p_rp);
    void ApplyCentralImpulse(const glm::vec3 &p_impulse);

    void ApplyTorque(const glm::vec3 &p_torque, bool p_impulse);

    void SetMotionType(unsigned char p_type);
    unsigned char GetMotionType() const;

    btRigidBody* GetRigidBody() const;

    bool IsActive() const;

    void Update(CollisionUpdateStage p_stage);

    // ROC::Transformable
    void SetScale(const glm::vec3 &p_val) override;
};

}

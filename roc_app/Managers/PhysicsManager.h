#pragma once
#include "Interfaces/IPhysicsManager.h"
#include "Managers/Manager.h"

namespace ROC
{

class Element;
class Collision;
class Model;

class PhysicsManager final : public IPhysicsManager, public Manager
{
    bool m_enabled;
    btDiscreteDynamicsWorld *m_dynamicWorld;
    btBroadphaseInterface *m_broadPhase;
    btDefaultCollisionConfiguration *m_collisionConfig;
    btCollisionDispatcher *m_dispatcher;
    btSequentialImpulseConstraintSolver *m_solver;
    float m_timeStep;

    btRigidBody *m_floorBody;
    std::vector<Collision*> m_collisions;

    PhysicsManager(const PhysicsManager &that) = delete;
    PhysicsManager& operator=(const PhysicsManager &that) = delete;

    // ROC::IPhysicsManager
    void SetCollisionScale(ICollision *p_col, const glm::vec3 &p_scale);
    bool RayCast(const glm::vec3 &p_start, glm::vec3 &p_end, glm::vec3 &p_normal, IElement *&p_element);
public:
    explicit PhysicsManager(Core *p_core);
    ~PhysicsManager() = default;

    // ROC::Manager
    void Start() override;
    void Stop() override;

    void SetPhysicsEnabled(bool p_value);
    bool GetPhysicsEnabled() const;

    void SetFloorEnabled(bool p_value);
    bool GetFloorEnabled() const;

    void SetGravity(const glm::vec3 &p_grav);
    void GetGravity(glm::vec3 &p_grav) const;

    void SetCollisionScale(Collision *p_col, const glm::vec3 &p_scale);
    bool RayCast(const glm::vec3 &p_start, glm::vec3 &p_end, glm::vec3 &p_normal, Element *&p_element);

    void UpdateWorldSteps(unsigned int p_fps);

    void AddModel(Model *p_model);
    void RemoveModel(Model *p_model);
    void AddCollision(Collision *p_col);
    void RemoveCollision(Collision *p_col);

    void SetDebugDrawer(btIDebugDraw *p_drawer);
    void DrawDebugWorld();

    void DoPulse();
};

}

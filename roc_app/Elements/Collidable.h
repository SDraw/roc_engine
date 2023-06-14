#pragma once
#include "Interfaces/ICollidable.h"

namespace ROC
{

class Collidable : public ICollidable
{
    Collidable(const Collidable &that) = delete;
    Collidable& operator=(const Collidable &that) = delete;

    // ROC::ICollidable
    bool SetCollidableWith(ICollidable *p_col, bool p_state) override;
public:
    bool SetCollidableWith(Collidable *p_col, bool p_state);
protected:
    Collidable() = default;
    virtual ~Collidable() = default;

    virtual void GetRigidBodies(std::vector<btRigidBody*> &p_vec) = 0;
};

}

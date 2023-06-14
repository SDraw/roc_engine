#include "stdafx.h"

#include "Elements/Collidable.h"

bool ROC::Collidable::SetCollidableWith(Collidable *p_col, bool p_state)
{
    bool l_result = false;
    std::vector<btRigidBody*> l_groupA;
    std::vector<btRigidBody*> l_groupB;

    this->GetRigidBodies(l_groupA);
    p_col->GetRigidBodies(l_groupB);

    if(!l_groupA.empty() && !l_groupB.empty())
    {
        for(auto l_bodyA : l_groupA)
        {
            for(auto l_bodyB : l_groupB)
            {
                l_bodyA->setIgnoreCollisionCheck(l_bodyB, !p_state);
                l_bodyB->setIgnoreCollisionCheck(l_bodyA, !p_state);
            }
        }
        l_result = true;
    }

    return l_result;
}

// ROC::ICollidable
bool ROC::Collidable::SetCollidableWith(ICollidable *p_col, bool p_state)
{
    return SetCollidableWith(dynamic_cast<Collidable*>(p_col), p_state);
}

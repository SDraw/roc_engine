#pragma once

namespace ROC
{

class ICollidable
{
public:
    virtual bool SetCollidableWith(ICollidable *p_col, bool p_state) = 0;
};

}

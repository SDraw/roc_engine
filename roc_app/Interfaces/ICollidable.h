#pragma once

namespace ROC
{

class ICollidable
{
protected:
    ~ICollidable() = default;
public:
    virtual bool SetCollidableWith(ICollidable *p_col, bool p_state) = 0;
};

}

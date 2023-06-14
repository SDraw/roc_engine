#pragma once

namespace ROC
{

class IAnimation
{
public:
    virtual size_t GetBonesCount() const = 0;
    virtual unsigned int GetDuration() const = 0;
};

}

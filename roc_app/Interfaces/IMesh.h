#pragma once

namespace ROC
{

class IMesh
{
protected:
    ~IMesh() = default;
public:
    virtual float GetBoundSphereRadius() const = 0;
    virtual size_t GetMaterialsCount() const = 0;
};

}

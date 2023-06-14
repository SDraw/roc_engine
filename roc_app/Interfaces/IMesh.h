#pragma once

namespace ROC
{

class IMesh
{
public:
    virtual float GetBoundSphereRadius() const = 0;
    virtual size_t GetMaterialsCount() const = 0;
};

}

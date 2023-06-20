#pragma once

namespace ROC
{

class ICustomArgument;

class ICustomArguments
{
protected:
    ~ICustomArguments() = default;
public:
    virtual size_t GetArgumentsCount() const = 0;
    virtual const ICustomArgument* GetArgument(size_t p_index) const = 0;
};

}

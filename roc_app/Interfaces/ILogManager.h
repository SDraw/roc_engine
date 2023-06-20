#pragma once

namespace ROC
{

class ILogManager
{
protected:
    ~ILogManager() = default;
public:
    virtual void Log(const char *p_text) = 0;
};

}

#pragma once

namespace ROC
{

class ILogManager
{
public:
    virtual void Log(const char *p_text) = 0;
};

}

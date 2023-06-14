#pragma once
#include "Managers/Manager.h"

namespace ROC
{

class ICore;
class IModule;
class CustomArguments;

using ModuleInitFunc = IModule* (*)(ICore*);

class ModuleManager final : public Manager
{
    std::vector<IModule*> m_modules;
    std::vector<HMODULE> m_libraries;
public:
    explicit ModuleManager(Core *p_core);
    ~ModuleManager() = default;

    // ROC::Manager
    void Start() override;
    void Stop() override;

    void SignalGlobalEvent(unsigned char p_event, const CustomArguments *p_args) const;

    void DoPulse() const;
};

}

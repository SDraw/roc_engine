#pragma once

class LuaHandler;
class Core final : public ROC::IModule
{
    static Core* ms_instance;

    ROC::ICore *m_engineCore;
    LuaHandler *m_luaHandler;

    Core();
    ~Core();
    Core(const Core &that) = delete;
    Core& operator=(const Core &that) = delete;

    void RecieveEvent(ModuleEvent p_event, const ROC::ICustomArguments *p_args) override;
    void DoPulse() override;
public:
    static void Init(ROC::ICore *p_core);
    static Core* GetInstance();

    ROC::ICore* GetEngineCore();
};


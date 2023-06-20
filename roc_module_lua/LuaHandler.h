#pragma once

class LuaVM;
class LuaHandler final
{
    LuaVM *m_vm;
    std::unordered_map<ROC::IModule::ModuleEvent, std::int32_t> m_eventFunctions;

    LuaHandler(const LuaHandler &that) = delete;
    LuaHandler& operator=(const LuaHandler &that) = delete;
public:
    LuaHandler();
    ~LuaHandler();

    static void InitDefs();

    void LoadScript(const std::string &p_path);

    void ParseEvents();

    void CallEvent(ROC::IModule::ModuleEvent p_event, const ROC::ICustomArguments *p_args);

    void PerformGC();
};


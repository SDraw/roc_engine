#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class TimeDefs final
{
    static std::vector<LuaPropDef> ms_staticProps;
    static std::vector<LuaMethodDef> ms_staticMethods;

    TimeDefs() = delete;
    ~TimeDefs() = delete;

    static int GetDeltaTime(lua_State *p_state);
    static int GetTotalTime(lua_State *p_state);
    static int GetSystemTime(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};


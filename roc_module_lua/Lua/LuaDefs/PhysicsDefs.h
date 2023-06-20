#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class PhysicsDefs final
{
    static std::vector<LuaPropDef> ms_staticProps;
    static std::vector<LuaMethodDef> ms_staticMethods;

    PhysicsDefs() = delete;
    ~PhysicsDefs() = delete;

    static int GetEnabled(lua_State *p_state);
    static int SetEnabled(lua_State *p_state);
    static int GetFloorEnabled(lua_State *p_state);
    static int SetFloorEnabled(lua_State *p_state);
    static int GetGravity(lua_State *p_state);
    static int SetGravity(lua_State *p_state);
    static int Raycast(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};

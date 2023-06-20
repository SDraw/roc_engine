#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class LightDefs final
{
    static std::vector<LuaMethodDef> ms_metaMethods;
    static std::vector<LuaPropDef> ms_instanceProps;

    LightDefs() = delete;
    ~LightDefs() = delete;

    static int Create(lua_State *p_state);
    static int GetType(lua_State *p_state);
    static int SetType(lua_State *p_state);
    static int GetDirection(lua_State *p_state);
    static int SetDirection(lua_State *p_state);
    static int GetColor(lua_State *p_state);
    static int SetColor(lua_State *p_state);
    static int GetCutoff(lua_State *p_state);
    static int SetCutoff(lua_State *p_state);
    static int GetFalloff(lua_State *p_state);
    static int SetFalloff(lua_State *p_state);
    static int IsLight(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};

#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class InputDefs final
{
    static std::vector<LuaPropDef> ms_staticProps;
    static std::vector<LuaMethodDef> ms_staticMethods;

    InputDefs() = delete;
    ~InputDefs() = delete;

    static int GetEnabled(lua_State *p_state);
    static int SetEnabled(lua_State *p_state);
    static int GetClipboard(lua_State *p_state);
    static int SetClipboard(lua_State *p_state);
    static int GetKey(lua_State *p_state);
    static int GetMouseKey(lua_State *p_state);
    static int IsJoypadConnected(lua_State *p_state);
    static int GetJoypadKey(lua_State *p_state);
    static int GetJoypadButtonCount(lua_State *p_state);
    static int IsJoypadAxisPresent(lua_State *p_state);
    static int GetJoypadAxis(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};


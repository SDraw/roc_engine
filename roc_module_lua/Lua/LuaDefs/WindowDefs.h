#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class WindowDefs final
{
    static std::vector<LuaPropDef> ms_staticProps;
    static std::vector<LuaMethodDef> ms_staticMethods;

    WindowDefs() = delete;
    ~WindowDefs() = delete;

    static int GetPosition(lua_State *p_state);
    static int SetPosition(lua_State *p_state);
    static int GetSize(lua_State *p_state);
    static int SetVSync(lua_State *p_state);
    static int GetFrameLimit(lua_State *p_state);
    static int SetFrameLimit(lua_State *p_state);
    static int SetTitle(lua_State *p_state);
    static int SetIcon(lua_State *p_state);
    static int GetFocus(lua_State *p_state);
    static int GetCursorPosition(lua_State *p_state);
    static int SetCursorPosition(lua_State *p_state);

    static int Close(lua_State *p_state);
    static int RequestFocus(lua_State *p_state);
    static int SetCursorMode(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};


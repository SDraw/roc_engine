#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class RenderDefs final
{
    static std::vector<LuaPropDef> ms_staticProps;
    static std::vector<LuaMethodDef> ms_staticMethods;

    RenderDefs() = delete;
    ~RenderDefs() = delete;

    static int SetPolygonMode(lua_State *p_state);
    static int SetClearColor(lua_State *p_state);
    static int SetViewport(lua_State *p_state);
    static int ClearViewport(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};


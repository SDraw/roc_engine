#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class RenderTargetDefs final
{
    static std::vector<LuaMethodDef> ms_metaMethods;
    static std::vector<LuaPropDef> ms_instanceProps;
    static std::vector<LuaMethodDef> ms_instanceMethods;

    RenderTargetDefs() = delete;
    ~RenderTargetDefs() = delete;

    static int Create(lua_State *p_state);
    static int SetProperty(lua_State *p_state);
    static int IsRenderTarget(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};

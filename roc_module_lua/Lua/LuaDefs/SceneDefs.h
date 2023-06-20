#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class SceneDefs final
{
    static std::vector<LuaMethodDef> ms_metaMethods;
    static std::vector<LuaPropDef> ms_instanceProps;
    static std::vector<LuaMethodDef> ms_instanceMethods;

    SceneDefs() = delete;
    ~SceneDefs() = delete;

    static int Create(lua_State *p_state);
    static int GetCamera(lua_State *p_state);
    static int SetCamera(lua_State *p_state);
    static int GetRenderTarget(lua_State *p_state);
    static int SetRenderTarget(lua_State *p_state);
    static int GetShader(lua_State *p_state);
    static int SetShader(lua_State *p_state);
    static int AddLight(lua_State *p_state);
    static int RemoveLight(lua_State *p_state);
    static int HasLight(lua_State *p_state);
    static int AddModel(lua_State *p_state);
    static int RemoveModel(lua_State *p_state);
    static int HasModel(lua_State *p_state);
    static int GetShadows(lua_State *p_state);
    static int SetShadows(lua_State *p_state);
    static int GetShadowsArea(lua_State *p_state);
    static int SetShadowsArea(lua_State *p_state);
    static int GetShadowsDepth(lua_State *p_state);
    static int SetShadowsDepth(lua_State *p_state);
    static int GetShadowsQuality(lua_State *p_state);
    static int SetShadowsQuality(lua_State *p_state);
    static int Draw(lua_State *p_state);
    static int IsScene(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};


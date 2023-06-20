#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class CameraDefs final
{
    static std::vector<LuaMethodDef> ms_metaMethods;
    static std::vector<LuaPropDef> ms_instanceProps;

    CameraDefs() = delete;
    ~CameraDefs() = delete;

    static int Create(lua_State *p_state);
    static int GetProjectionType(lua_State *p_state);
    static int SetProjectionType(lua_State *p_state);
    static int GetFov(lua_State *p_state);
    static int SetFov(lua_State *p_state);
    static int GetAspectRatio(lua_State *p_state);
    static int SetAspectRatio(lua_State *p_state);
    static int GetOrthoParams(lua_State *p_state);
    static int SetOrthoParams(lua_State *p_state);
    static int GetDepth(lua_State *p_state);
    static int SetDepth(lua_State *p_state);
    static int GetDirection(lua_State *p_state);
    static int SetDirection(lua_State *p_state);
    static int GetUp(lua_State *p_state);
    static int SetUp(lua_State *p_state);
    static int GetViewMatrix(lua_State *p_state);
    static int GetProjectionMatrix(lua_State *p_state);
    static int GetViewProjectionMatrix(lua_State *p_state);
    static int IsCamera(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};


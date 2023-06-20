#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class ModelDefs final
{
    static std::vector<LuaMethodDef> ms_metaMethods;
    static std::vector<LuaPropDef> ms_instanceProps;
    static std::vector<LuaMethodDef> ms_instanceMethods;

    ModelDefs() = default;
    ~ModelDefs() = default;

    static int Create(lua_State *p_state);
    static int GetMesh(lua_State *p_state);
    static int GetBoundSphereRadius(lua_State *p_state);
    static int GetAnimation(lua_State *p_state);
    static int SetAnimation(lua_State *p_state);
    static int GetIsVisible(lua_State *p_state);
    static int PlayAnimation(lua_State *p_state);
    static int PauseAnimation(lua_State *p_state);
    static int ResetAnimation(lua_State *p_state);
    static int GetAnimationProperty(lua_State *p_state);
    static int SetAnimationProperty(lua_State *p_state);
    static int IsModel(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};


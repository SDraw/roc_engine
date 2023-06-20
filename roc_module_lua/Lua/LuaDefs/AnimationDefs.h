#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class AnimationDefs final
{
    static std::vector<LuaMethodDef> ms_metaMethods;
    static std::vector<LuaPropDef> ms_instanceProps;

    AnimationDefs() = delete;
    ~AnimationDefs() = delete;

    static int Create(lua_State *p_state);
    static int GetBonesCount(lua_State *p_state);
    static int GetDuration(lua_State *p_state);
    static int IsAnimation(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};

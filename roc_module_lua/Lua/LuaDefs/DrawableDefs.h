#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class DrawableDefs final
{
    static std::vector<LuaPropDef> ms_instanceProps;
    static std::vector<LuaMethodDef> ms_instanceMethods;

    DrawableDefs() = delete;
    ~DrawableDefs() = delete;

    static int GetIsTransparent(lua_State *p_state);
    static int GetIsCubic(lua_State *p_state);
    static int GetSize(lua_State *p_state);
    static int GetFiltering(lua_State *p_state);
    static int Draw(lua_State *p_state);
    static int IsDrawable(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
    static void InheritTo(std::vector< LuaPropDef> *p_instanceProps, std::vector<LuaMethodDef> *p_instanceMethods);
};

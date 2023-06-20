#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class FontDefs final
{
    static std::vector<LuaMethodDef> ms_metaMethods;
    static std::vector<LuaPropDef> ms_instanceProps;
    static std::vector<LuaMethodDef> ms_instanceMethods;

    FontDefs() = delete;
    ~FontDefs() = delete;

    static int Create(lua_State *p_state);
    static int GetFiltering(lua_State *p_state);
    static int GetGlyphSize(lua_State *p_state);
    static int Draw(lua_State *p_state);
    static int IsFont(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};

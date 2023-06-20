#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class TextureDefs final
{
    static std::vector<LuaMethodDef> ms_metaMethods;
    static std::vector<LuaPropDef> ms_instanceProps;
    static std::vector<LuaMethodDef> ms_instanceMethods;

    TextureDefs() = delete;
    ~TextureDefs() = delete;

    static int Create(lua_State *p_state);
    static int GetIsCompressed(lua_State *p_state);
    static int IsTexture(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};


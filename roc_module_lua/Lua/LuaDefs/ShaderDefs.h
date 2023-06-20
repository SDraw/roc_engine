#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class ShaderDefs final
{
    static std::vector<LuaMethodDef> ms_metaMethods;
    static std::vector<LuaPropDef> ms_instanceProps;
    static std::vector<LuaMethodDef> ms_instanceMethods;

    ShaderDefs() = delete;
    ~ShaderDefs() = delete;

    static int Create(lua_State *p_state);
    static int Attach(lua_State *p_state);
    static int Detach(lua_State *p_state);
    static int HasAttached(lua_State *p_state);
    static int SetUniform(lua_State *p_state);
    static int IsShader(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};


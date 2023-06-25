#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class Matrix2Defs final
{
    static std::vector<LuaPropDef> ms_staticProps;
    static std::vector<LuaMethodDef> ms_staticMethods;
    static std::vector<LuaMethodDef> ms_metaMethods;
    static std::vector<LuaPropDef> ms_instanceProps;
    static std::vector<LuaMethodDef> ms_instanceMethods;

    Matrix2Defs() = delete;
    ~Matrix2Defs() = delete;

    static int Create(lua_State *p_state);
    static int GetZero(lua_State *p_state);
    static int GetIdentity(lua_State *p_state);
    static int Add(lua_State *p_state);
    static int Subtract(lua_State *p_state);
    static int Divide(lua_State *p_state);
    static int Multiply(lua_State *p_state);
    static int GetDeterminant(lua_State *p_state);
    static int GetInverse(lua_State *p_state);
    static int IsMatrix2(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};

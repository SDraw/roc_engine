#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class Vector2Defs final
{
    static std::vector<LuaPropDef> ms_staticProps;
    static std::vector<LuaMethodDef> ms_staticMethods;
    static std::vector<LuaMethodDef> ms_metaMethods;
    static std::vector<LuaPropDef> ms_instanceProps;
    static std::vector<LuaMethodDef> ms_instanceMethods;

    Vector2Defs() = delete;
    ~Vector2Defs() = delete;

    static int Create(lua_State *p_state);
    static int GetZero(lua_State *p_state);
    static int GetOne(lua_State *p_state);
    static int Add(lua_State *p_state);
    static int Subtract(lua_State *p_state);
    static int Divide(lua_State *p_state);
    static int Multiply(lua_State *p_state);
    static int GetLength(lua_State *p_state);
    static int GetX(lua_State *p_state);
    static int SetX(lua_State *p_state);
    static int GetY(lua_State *p_state);
    static int SetY(lua_State *p_state);
    static int IsVector2(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};

#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class Vector3Defs final
{
    static std::vector<LuaPropDef> ms_staticProps;
    static std::vector<LuaMethodDef> ms_staticMethods;
    static std::vector<LuaMethodDef> ms_metaMethods;
    static std::vector<LuaPropDef> ms_instanceProps;
    static std::vector<LuaMethodDef> ms_instanceMethods;

    Vector3Defs() = delete;
    ~Vector3Defs() = delete;

    static int Create(lua_State *p_state);
    static int GetZero(lua_State *p_state);
    static int GetOne(lua_State *p_state);
    static int GetLeft(lua_State *p_state);
    static int GetRight(lua_State *p_state);
    static int GetUp(lua_State *p_state);
    static int GetDown(lua_State *p_state);
    static int GetForward(lua_State *p_state);
    static int GetBack(lua_State *p_state);
    static int Angle(lua_State *p_state);
    static int Cross(lua_State *p_state);
    static int Distance(lua_State *p_state);
    static int Dot(lua_State *p_state);
    static int Lerp(lua_State *p_state);
    static int Reflect(lua_State *p_state);
    static int Slerp(lua_State *p_state);
    static int Add(lua_State *p_state);
    static int Subtract(lua_State *p_state);
    static int Divide(lua_State *p_state);
    static int Multiply(lua_State *p_state);
    static int GetLength(lua_State *p_state);
    static int GetX(lua_State *p_state);
    static int SetX(lua_State *p_state);
    static int GetY(lua_State *p_state);
    static int SetY(lua_State *p_state);
    static int GetZ(lua_State *p_state);
    static int SetZ(lua_State *p_state);
    static int IsVector3(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};

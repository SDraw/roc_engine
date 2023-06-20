#include "stdafx.h"
#include "Lua/LuaDefs/Vector2Defs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaArgReader.h"
#include "Utils.h"

extern const std::string g_vec2Name("Vector2");

std::vector<LuaPropDef> Vector2Defs::ms_staticProps;
std::vector<LuaMethodDef> Vector2Defs::ms_staticMethods;
std::vector<LuaMethodDef> Vector2Defs::ms_metaMethods;
std::vector<LuaPropDef> Vector2Defs::ms_instanceProps;
std::vector<LuaMethodDef> Vector2Defs::ms_instanceMethods;

void Vector2Defs::Init()
{
    ms_staticProps.emplace_back("one", GetOne, nullptr);
    ms_staticProps.emplace_back("zero", GetZero, nullptr);

    ms_metaMethods.emplace_back("__add", Add);
    ms_metaMethods.emplace_back("__sub", Subtract);
    ms_metaMethods.emplace_back("__div", Divide);
    ms_metaMethods.emplace_back("__mul", Multiply);
    ms_metaMethods.emplace_back("__len", GetLength);

    ms_instanceProps.emplace_back("x", GetX, SetX);
    ms_instanceProps.emplace_back("y", GetY, SetY);
}

void Vector2Defs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_vec2Name, Create, &ms_staticProps, &ms_staticMethods, &ms_metaMethods, &ms_instanceProps, &ms_instanceMethods);
    p_vm->RegisterFunction("isVector2", IsVector2);
}

int Vector2Defs::Create(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    float l_x = 0.f;
    float l_y = 0.f;
    l_argReader.Skip();
    l_argReader.ReadNextNumber(l_x);
    l_argReader.ReadNextNumber(l_y);
    l_argReader.PushObject(new glm::vec2(l_x, l_y), g_vec2Name, false);
    return l_argReader.GetReturnValue();
}

int Vector2Defs::GetOne(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::vec2(1.f), g_vec2Name, false);
    return 1;
}
int Vector2Defs::GetZero(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::vec2(0.f), g_vec2Name, false);
    return 1;
}

int Vector2Defs::Add(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec2 *l_vecA;
    glm::vec2 *l_vecB;
    l_argReader.ReadObject(l_vecA, g_vec2Name);
    l_argReader.ReadObject(l_vecB, g_vec2Name);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::vec2(*l_vecA + *l_vecB), g_vec2Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector2Defs::Subtract(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec2 *l_vecA;
    glm::vec2 *l_vecB;
    l_argReader.ReadObject(l_vecA, g_vec2Name);
    l_argReader.ReadObject(l_vecB, g_vec2Name);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::vec2(*l_vecA - *l_vecB), g_vec2Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector2Defs::Divide(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec2 *l_vecA;
    float l_val;
    l_argReader.ReadObject(l_vecA, g_vec2Name);
    l_argReader.ReadNumber(l_val);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::vec2(*l_vecA / l_val), g_vec2Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector2Defs::Multiply(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec2 *l_vecA;
    float l_val;
    if(l_argReader.IsNextObject())
    {
        l_argReader.ReadObject(l_vecA, g_vec2Name);
        l_argReader.ReadNumber(l_val);
    }
    else
    {
        l_argReader.ReadNumber(l_val);
        l_argReader.ReadObject(l_vecA, g_vec2Name);
    }
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::vec2(*l_vecA * l_val), g_vec2Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector2Defs::GetLength(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec2 *l_vec;
    l_argReader.ReadObject(l_vec, g_vec2Name);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(glm::length(*l_vec));
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int Vector2Defs::GetX(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec2 *l_vec;
    l_argReader.ReadObject(l_vec, g_vec2Name);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(l_vec->x);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector2Defs::SetX(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec2 *l_vec;
    float l_value;
    l_argReader.ReadObject(l_vec, g_vec2Name);
    l_argReader.ReadNumber(l_value);
    if(!l_argReader.HasError())
        l_vec->x = l_value;

    l_argReader.LogError();
    return 0;
}

int Vector2Defs::GetY(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec2 *l_vec;
    l_argReader.ReadObject(l_vec, g_vec2Name);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(l_vec->y);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector2Defs::SetY(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec2 *l_vec;
    float l_value;
    l_argReader.ReadObject(l_vec, g_vec2Name);
    l_argReader.ReadNumber(l_value);
    if(!l_argReader.HasError())
        l_vec->y = l_value;

    l_argReader.LogError();
    return 0;
}

int Vector2Defs::IsVector2(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec2 *l_vec = nullptr;
    l_argReader.ReadNextObject(l_vec, g_vec2Name);
    l_argReader.PushBoolean(l_vec != nullptr);
    return l_argReader.GetReturnValue();
}

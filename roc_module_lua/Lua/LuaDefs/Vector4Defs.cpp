#include "stdafx.h"
#include "Lua/LuaDefs/Vector4Defs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaArgReader.h"
#include "Utils.h"

extern const std::string g_vec4Name("Vector4");

std::vector<LuaPropDef> Vector4Defs::ms_staticProps;
std::vector<LuaMethodDef> Vector4Defs::ms_staticMethods;
std::vector<LuaMethodDef> Vector4Defs::ms_metaMethods;
std::vector<LuaPropDef> Vector4Defs::ms_instanceProps;
std::vector<LuaMethodDef> Vector4Defs::ms_instanceMethods;

void Vector4Defs::Init()
{
    ms_staticProps.emplace_back("one", GetOne, nullptr);
    ms_staticProps.emplace_back("zero", GetZero, nullptr);

    ms_staticMethods.emplace_back("distance", Distance);
    ms_staticMethods.emplace_back("dot", Dot);
    ms_staticMethods.emplace_back("lerp", Lerp);

    ms_metaMethods.emplace_back("__add", Add);
    ms_metaMethods.emplace_back("__sub", Subtract);
    ms_metaMethods.emplace_back("__div", Divide);
    ms_metaMethods.emplace_back("__mul", Multiply);
    ms_metaMethods.emplace_back("__len", GetLength);

    ms_instanceProps.emplace_back("x", GetX, SetX);
    ms_instanceProps.emplace_back("y", GetY, SetY);
    ms_instanceProps.emplace_back("z", GetZ, SetZ);
    ms_instanceProps.emplace_back("w", GetW, SetW);
}

void Vector4Defs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_vec4Name, Create, &ms_staticProps, &ms_staticMethods, &ms_metaMethods, &ms_instanceProps, &ms_instanceMethods);
    p_vm->RegisterFunction("isVector4", IsVector4);
}

int Vector4Defs::Create(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    float l_x = 0.f;
    float l_y = 0.f;
    float l_z = 0.f;
    float l_w = 0.f;
    l_argReader.Skip();
    l_argReader.ReadNextNumber(l_x);
    l_argReader.ReadNextNumber(l_y);
    l_argReader.ReadNextNumber(l_z);
    l_argReader.ReadNextNumber(l_w);
    l_argReader.PushObject(new glm::vec4(l_x, l_y, l_z, l_w), g_vec4Name, false);
    return l_argReader.GetReturnValue();
}

int Vector4Defs::GetOne(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::vec4(1.f), g_vec4Name, false);
    return 1;
}
int Vector4Defs::GetZero(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::vec4(0.f), g_vec4Name, false);
    return 1;
}

int Vector4Defs::Distance(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec4 *l_vecA;
    glm::vec4 *l_vecB;
    l_argReader.ReadObject(l_vecA, g_vec4Name);
    l_argReader.ReadObject(l_vecB, g_vec4Name);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(glm::distance(*l_vecA, *l_vecB));
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int Vector4Defs::Dot(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec4 *l_vecA;
    glm::vec4 *l_vecB;
    l_argReader.ReadObject(l_vecA, g_vec4Name);
    l_argReader.ReadObject(l_vecB, g_vec4Name);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(glm::dot(*l_vecA, *l_vecB));
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int Vector4Defs::Lerp(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec4 *l_vecA;
    glm::vec4 *l_vecB;
    float l_alpha;
    l_argReader.ReadObject(l_vecA, g_vec4Name);
    l_argReader.ReadObject(l_vecB, g_vec4Name);
    l_argReader.ReadNumber(l_alpha);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::vec4(glm::lerp(*l_vecA, *l_vecB,l_alpha)), g_vec4Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int Vector4Defs::Add(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec4 *l_vecA;
    glm::vec4 *l_vecB;
    l_argReader.ReadObject(l_vecA, g_vec4Name);
    l_argReader.ReadObject(l_vecB, g_vec4Name);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::vec4(*l_vecA + *l_vecB), g_vec4Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector4Defs::Subtract(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec4 *l_vecA;
    glm::vec4 *l_vecB;
    l_argReader.ReadObject(l_vecA, g_vec4Name);
    l_argReader.ReadObject(l_vecB, g_vec4Name);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::vec4(*l_vecA - *l_vecB), g_vec4Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector4Defs::Divide(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec4 *l_vecA;
    float l_val;
    l_argReader.ReadObject(l_vecA, g_vec4Name);
    l_argReader.ReadNumber(l_val);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::vec4(*l_vecA / l_val), g_vec4Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector4Defs::Multiply(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec4 *l_vecA;
    float l_val;
    if(l_argReader.IsNextObject())
    {
        l_argReader.ReadObject(l_vecA, g_vec4Name);
        l_argReader.ReadNumber(l_val);
    }
    else
    {
        l_argReader.ReadNumber(l_val);
        l_argReader.ReadObject(l_vecA, g_vec4Name);
    }
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::vec4(*l_vecA * l_val), g_vec4Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector4Defs::GetLength(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec4 *l_vec;
    l_argReader.ReadObject(l_vec, g_vec4Name);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(glm::length(*l_vec));
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int Vector4Defs::GetX(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec4 *l_vec;
    l_argReader.ReadObject(l_vec, g_vec4Name);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(l_vec->x);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector4Defs::SetX(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec4 *l_vec;
    float l_value;
    l_argReader.ReadObject(l_vec, g_vec4Name);
    l_argReader.ReadNumber(l_value);
    if(!l_argReader.HasError())
        l_vec->x = l_value;

    l_argReader.LogError();
    return 0;
}

int Vector4Defs::GetY(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec4 *l_vec;
    l_argReader.ReadObject(l_vec, g_vec4Name);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(l_vec->y);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector4Defs::SetY(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec4 *l_vec;
    float l_value;
    l_argReader.ReadObject(l_vec, g_vec4Name);
    l_argReader.ReadNumber(l_value);
    if(!l_argReader.HasError())
        l_vec->y = l_value;

    l_argReader.LogError();
    return 0;
}

int Vector4Defs::GetZ(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec4 *l_vec;
    l_argReader.ReadObject(l_vec, g_vec4Name);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(l_vec->z);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector4Defs::SetZ(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec4 *l_vec;
    float l_value;
    l_argReader.ReadObject(l_vec, g_vec4Name);
    l_argReader.ReadNumber(l_value);
    if(!l_argReader.HasError())
        l_vec->z = l_value;

    l_argReader.LogError();
    return 0;
}

int Vector4Defs::GetW(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec4 *l_vec;
    l_argReader.ReadObject(l_vec, g_vec4Name);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(l_vec->w);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector4Defs::SetW(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec4 *l_vec;
    float l_value;
    l_argReader.ReadObject(l_vec, g_vec4Name);
    l_argReader.ReadNumber(l_value);
    if(!l_argReader.HasError())
        l_vec->w = l_value;

    l_argReader.LogError();
    return 0;
}

int Vector4Defs::IsVector4(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec4 *l_vec = nullptr;
    l_argReader.ReadNextObject(l_vec, g_vec4Name);
    l_argReader.PushBoolean(l_vec != nullptr);
    return l_argReader.GetReturnValue();
}

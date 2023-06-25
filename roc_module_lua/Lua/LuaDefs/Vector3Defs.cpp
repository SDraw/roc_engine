#include "stdafx.h"
#include "Lua/LuaDefs/Vector3Defs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaArgReader.h"
#include "Utils.h"

extern const std::string g_vec3Name("Vector3");

std::vector<LuaPropDef> Vector3Defs::ms_staticProps;
std::vector<LuaMethodDef> Vector3Defs::ms_staticMethods;
std::vector<LuaMethodDef> Vector3Defs::ms_metaMethods;
std::vector<LuaPropDef> Vector3Defs::ms_instanceProps;
std::vector<LuaMethodDef> Vector3Defs::ms_instanceMethods;

void Vector3Defs::Init()
{
    ms_staticProps.emplace_back("one", GetOne, nullptr);
    ms_staticProps.emplace_back("zero", GetZero, nullptr);
    ms_staticProps.emplace_back("left", GetLeft, nullptr);
    ms_staticProps.emplace_back("right", GetRight, nullptr);
    ms_staticProps.emplace_back("up", GetUp, nullptr);
    ms_staticProps.emplace_back("down", GetDown, nullptr);
    ms_staticProps.emplace_back("forward", GetForward, nullptr);
    ms_staticProps.emplace_back("back", GetBack, nullptr);

    ms_staticMethods.emplace_back("angle", Angle);
    ms_staticMethods.emplace_back("cross", Cross);
    ms_staticMethods.emplace_back("distance", Distance);
    ms_staticMethods.emplace_back("dot", Dot);
    ms_staticMethods.emplace_back("lerp", Lerp);
    ms_staticMethods.emplace_back("reflect", Reflect);
    ms_staticMethods.emplace_back("slerp", Slerp);

    ms_metaMethods.emplace_back("__add", Add);
    ms_metaMethods.emplace_back("__sub", Subtract);
    ms_metaMethods.emplace_back("__div", Divide);
    ms_metaMethods.emplace_back("__mul", Multiply);
    ms_metaMethods.emplace_back("__len", GetLength);

    ms_instanceProps.emplace_back("x", GetX, SetX);
    ms_instanceProps.emplace_back("y", GetY, SetY);
    ms_instanceProps.emplace_back("z", GetZ, SetZ);
}

void Vector3Defs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_vec3Name, Create, &ms_staticProps, &ms_staticMethods, &ms_metaMethods, &ms_instanceProps, &ms_instanceMethods);
    p_vm->RegisterFunction("isVector3", IsVector3);
}

int Vector3Defs::Create(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    float l_x = 0.f;
    float l_y = 0.f;
    float l_z = 0.f;
    l_argReader.Skip();
    l_argReader.ReadNextNumber(l_x);
    l_argReader.ReadNextNumber(l_y);
    l_argReader.ReadNextNumber(l_z);
    l_argReader.PushObject(new glm::vec3(l_x, l_y, l_z), g_vec3Name, false);
    return l_argReader.GetReturnValue();
}

int Vector3Defs::GetOne(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::vec3(1.f), g_vec3Name, false);
    return 1;
}
int Vector3Defs::GetZero(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::vec3(0.f), g_vec3Name, false);
    return 1;
}
int Vector3Defs::GetLeft(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::vec3(-1.f, 0.f, 0.f), g_vec3Name, false);
    return 1;
}
int Vector3Defs::GetRight(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::vec3(1.f, 0.f, 0.f), g_vec3Name, false);
    return 1;
}
int Vector3Defs::GetUp(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::vec3(0.f, 1.f, 0.f), g_vec3Name, false);
    return 1;
}
int Vector3Defs::GetDown(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::vec3(0.f, -1.f, 0.f), g_vec3Name, false);
    return 1;
}
int Vector3Defs::GetForward(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::vec3(0.f, 0.f, -1.f), g_vec3Name, false);
    return 1;
}
int Vector3Defs::GetBack(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::vec3(0.f, 0.f, 1.f), g_vec3Name, false);
    return 1;
}

int Vector3Defs::Angle(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vecA;
    glm::vec3 *l_vecB;
    l_argReader.ReadObject(l_vecA, g_vec3Name);
    l_argReader.ReadObject(l_vecB, g_vec3Name);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(glm::angle(*l_vecA, *l_vecB));
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int Vector3Defs::Cross(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vecA;
    glm::vec3 *l_vecB;
    l_argReader.ReadObject(l_vecA, g_vec3Name);
    l_argReader.ReadObject(l_vecB, g_vec3Name);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::vec3(glm::cross(*l_vecA, *l_vecB)), g_vec3Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int Vector3Defs::Distance(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vecA;
    glm::vec3 *l_vecB;
    l_argReader.ReadObject(l_vecA, g_vec3Name);
    l_argReader.ReadObject(l_vecB, g_vec3Name);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(glm::distance(*l_vecA, *l_vecB));
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int Vector3Defs::Dot(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vecA;
    glm::vec3 *l_vecB;
    l_argReader.ReadObject(l_vecA, g_vec3Name);
    l_argReader.ReadObject(l_vecB, g_vec3Name);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(glm::dot(*l_vecA, *l_vecB));
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int Vector3Defs::Lerp(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vecA;
    glm::vec3 *l_vecB;
    float l_alpha;
    l_argReader.ReadObject(l_vecA, g_vec3Name);
    l_argReader.ReadObject(l_vecB, g_vec3Name);
    l_argReader.ReadNumber(l_alpha);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::vec3(glm::lerp(*l_vecA, *l_vecB, l_alpha)), g_vec3Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int Vector3Defs::Reflect(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vecA;
    glm::vec3 *l_vecB;
    l_argReader.ReadObject(l_vecA, g_vec3Name);
    l_argReader.ReadObject(l_vecB, g_vec3Name);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::vec3(glm::reflect(*l_vecA, *l_vecB)), g_vec3Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int Vector3Defs::Slerp(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vecA;
    glm::vec3 *l_vecB;
    float l_alpha;
    l_argReader.ReadObject(l_vecA, g_vec3Name);
    l_argReader.ReadObject(l_vecB, g_vec3Name);
    l_argReader.ReadNumber(l_alpha);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::vec3(glm::slerp(*l_vecA, *l_vecB, l_alpha)), g_vec3Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int Vector3Defs::Add(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vecA;
    glm::vec3 *l_vecB;
    l_argReader.ReadObject(l_vecA, g_vec3Name);
    l_argReader.ReadObject(l_vecB, g_vec3Name);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::vec3(*l_vecA + *l_vecB), g_vec3Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector3Defs::Subtract(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vecA;
    glm::vec3 *l_vecB;
    l_argReader.ReadObject(l_vecA, g_vec3Name);
    l_argReader.ReadObject(l_vecB, g_vec3Name);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::vec3(*l_vecA - *l_vecB), g_vec3Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector3Defs::Divide(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vecA;
    float l_val;
    l_argReader.ReadObject(l_vecA, g_vec3Name);
    l_argReader.ReadNumber(l_val);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::vec3(*l_vecA / l_val), g_vec3Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector3Defs::Multiply(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vecA;
    float l_val;
    if(l_argReader.IsNextObject())
    {
        l_argReader.ReadObject(l_vecA, g_vec3Name);
        l_argReader.ReadNumber(l_val);
    }
    else
    {
        l_argReader.ReadNumber(l_val);
        l_argReader.ReadObject(l_vecA, g_vec3Name);
    }
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::vec3(*l_vecA * l_val), g_vec3Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector3Defs::GetLength(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vec;
    l_argReader.ReadObject(l_vec, g_vec3Name);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(glm::length(*l_vec));
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int Vector3Defs::GetX(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vec;
    l_argReader.ReadObject(l_vec, g_vec3Name);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(l_vec->x);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector3Defs::SetX(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vec;
    float l_value;
    l_argReader.ReadObject(l_vec, g_vec3Name);
    l_argReader.ReadNumber(l_value);
    if(!l_argReader.HasError())
        l_vec->x = l_value;

    l_argReader.LogError();
    return 0;
}

int Vector3Defs::GetY(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vec;
    l_argReader.ReadObject(l_vec, g_vec3Name);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(l_vec->y);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector3Defs::SetY(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vec;
    float l_value;
    l_argReader.ReadObject(l_vec, g_vec3Name);
    l_argReader.ReadNumber(l_value);
    if(!l_argReader.HasError())
        l_vec->y = l_value;

    l_argReader.LogError();
    return 0;
}

int Vector3Defs::GetZ(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vec;
    l_argReader.ReadObject(l_vec, g_vec3Name);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(l_vec->z);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Vector3Defs::SetZ(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vec;
    float l_value;
    l_argReader.ReadObject(l_vec, g_vec3Name);
    l_argReader.ReadNumber(l_value);
    if(!l_argReader.HasError())
        l_vec->z = l_value;

    l_argReader.LogError();
    return 0;
}

int Vector3Defs::IsVector3(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vec = nullptr;
    l_argReader.ReadNextObject(l_vec, g_vec3Name);
    l_argReader.PushBoolean(l_vec != nullptr);
    return l_argReader.GetReturnValue();
}

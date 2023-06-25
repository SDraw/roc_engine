#include "stdafx.h"
#include "Lua/LuaDefs/QuaternionDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaArgReader.h"
#include "Utils.h"

extern const std::string g_vec3Name;
extern const std::string g_quatName("Quaternion");

std::vector<LuaPropDef> QuaternionDefs::ms_staticProps;
std::vector<LuaMethodDef> QuaternionDefs::ms_staticMethods;
std::vector<LuaMethodDef> QuaternionDefs::ms_metaMethods;
std::vector<LuaPropDef> QuaternionDefs::ms_instanceProps;
std::vector<LuaMethodDef> QuaternionDefs::ms_instanceMethods;

void QuaternionDefs::Init()
{
    ms_staticProps.emplace_back("identity", GetIdentity, nullptr);

    ms_staticMethods.emplace_back("euler", Euler);

    ms_metaMethods.emplace_back("__mul", Multiply);

    ms_instanceProps.emplace_back("x", GetX, SetX);
    ms_instanceProps.emplace_back("y", GetY, SetY);
    ms_instanceProps.emplace_back("z", GetZ, SetZ);
    ms_instanceProps.emplace_back("w", GetW, SetW);

    ms_instanceMethods.emplace_back("rotate", Rotate);
}

void QuaternionDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_quatName, Create, &ms_staticProps, &ms_staticMethods, &ms_metaMethods, &ms_instanceProps, &ms_instanceMethods);
    p_vm->RegisterFunction("isQuaternion", IsQuaternion);
}

int QuaternionDefs::Create(lua_State *p_state)
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
    l_argReader.PushObject(new glm::quat(l_w, l_x, l_y, l_z), g_quatName, false);
    return l_argReader.GetReturnValue();
}

int QuaternionDefs::GetIdentity(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::quat(1.f, 0.f, 0.f, 0.f), g_quatName, false);
    return 1;
}

int QuaternionDefs::Euler(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_vec;
    l_argReader.ReadObject(l_vec, g_vec3Name);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::quat(*l_vec), g_quatName, false);
    else
            l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int QuaternionDefs::Multiply(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::quat *l_quatA;
    glm::quat *l_quatB;
    l_argReader.ReadObject(l_quatA, g_quatName);
    l_argReader.ReadObject(l_quatB, g_quatName);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::quat(*l_quatA * *l_quatB), g_quatName, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int QuaternionDefs::GetX(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::quat *l_quat;
    l_argReader.ReadObject(l_quat, g_quatName);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(l_quat->x);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int QuaternionDefs::SetX(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::quat *l_quat;
    float l_value;
    l_argReader.ReadObject(l_quat, g_quatName);
    l_argReader.ReadNumber(l_value);
    if(!l_argReader.HasError())
        l_quat->x = l_value;

    l_argReader.LogError();
    return 0;
}

int QuaternionDefs::GetY(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::quat *l_quat;
    l_argReader.ReadObject(l_quat, g_quatName);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(l_quat->y);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int QuaternionDefs::SetY(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::quat *l_quat;
    float l_value;
    l_argReader.ReadObject(l_quat, g_quatName);
    l_argReader.ReadNumber(l_value);
    if(!l_argReader.HasError())
        l_quat->y = l_value;

    l_argReader.LogError();
    return 0;
}

int QuaternionDefs::GetZ(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::quat *l_quat;
    l_argReader.ReadObject(l_quat, g_quatName);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(l_quat->z);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int QuaternionDefs::SetZ(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::quat *l_quat;
    float l_value;
    l_argReader.ReadObject(l_quat, g_quatName);
    l_argReader.ReadNumber(l_value);
    if(!l_argReader.HasError())
        l_quat->z = l_value;

    l_argReader.LogError();
    return 0;
}

int QuaternionDefs::GetW(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::quat *l_quat;
    l_argReader.ReadObject(l_quat, g_quatName);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(l_quat->w);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int QuaternionDefs::SetW(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::quat *l_quat;
    float l_value;
    l_argReader.ReadObject(l_quat, g_quatName);
    l_argReader.ReadNumber(l_value);
    if(!l_argReader.HasError())
        l_quat->w = l_value;

    l_argReader.LogError();
    return 0;
}

int QuaternionDefs::Rotate(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::quat *l_quat;
    glm::vec3 *l_vec;
    l_argReader.ReadObject(l_quat, g_quatName);
    l_argReader.ReadObject(l_vec, g_vec3Name);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::vec3(*l_quat * *l_vec), g_vec3Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int QuaternionDefs::IsQuaternion(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::quat *l_quat = nullptr;
    l_argReader.ReadNextObject(l_quat, g_quatName);
    l_argReader.PushBoolean(l_quat != nullptr);
    return l_argReader.GetReturnValue();
}

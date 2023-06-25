#include "stdafx.h"
#include "Lua/LuaDefs/Matrix4Defs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaArgReader.h"
#include "Utils.h"

extern const std::string g_matrix4Name("Matrix4");

std::vector<LuaPropDef> Matrix4Defs::ms_staticProps;
std::vector<LuaMethodDef> Matrix4Defs::ms_staticMethods;
std::vector<LuaMethodDef> Matrix4Defs::ms_metaMethods;
std::vector<LuaPropDef> Matrix4Defs::ms_instanceProps;
std::vector<LuaMethodDef> Matrix4Defs::ms_instanceMethods;

void Matrix4Defs::Init()
{
    ms_staticProps.emplace_back("identity", GetIdentity, nullptr);
    ms_staticProps.emplace_back("zero", GetZero, nullptr);

    ms_metaMethods.emplace_back("__add", Add);
    ms_metaMethods.emplace_back("__sub", Subtract);
    ms_metaMethods.emplace_back("__div", Divide);
    ms_metaMethods.emplace_back("__mul", Multiply);
    ms_metaMethods.emplace_back("__len", GetDeterminant);

    ms_instanceProps.emplace_back("inverse", GetInverse, nullptr);
}

void Matrix4Defs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_matrix4Name, Create, &ms_staticProps, &ms_staticMethods, &ms_metaMethods, &ms_instanceProps, &ms_instanceMethods);
    p_vm->RegisterFunction("isMatrix4", IsMatrix4);
}

int Matrix4Defs::Create(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    float l_val[16] = { 0.f };
    l_argReader.Skip();
    for(size_t i = 0U; i < 16U; i++)
        l_argReader.ReadNextNumber(l_val[i]);
    l_argReader.PushObject(
        new glm::mat4(
            l_val[0], l_val[1], l_val[2], l_val[3],
            l_val[4], l_val[5], l_val[6], l_val[7],
            l_val[8], l_val[9], l_val[10], l_val[11],
            l_val[12], l_val[13], l_val[14], l_val[15]),
        g_matrix4Name, false
    );
    return l_argReader.GetReturnValue();
}

int Matrix4Defs::GetIdentity(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::mat4(1.f), g_matrix4Name, false);
    return 1;
}
int Matrix4Defs::GetZero(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::mat4(0.f), g_matrix4Name, false);
    return 1;
}

int Matrix4Defs::Add(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::mat4 *l_matA = nullptr;
    glm::mat4 *l_matB = nullptr;
    l_argReader.ReadObject(l_matA, g_matrix4Name);
    l_argReader.ReadObject(l_matB, g_matrix4Name);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::mat4(*l_matA + *l_matB), g_matrix4Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Matrix4Defs::Subtract(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::mat4 *l_matA = nullptr;
    glm::mat4 *l_matB = nullptr;
    l_argReader.ReadObject(l_matA, g_matrix4Name);
    l_argReader.ReadObject(l_matB, g_matrix4Name);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::mat4(*l_matA - *l_matB), g_matrix4Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Matrix4Defs::Divide(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::mat4 *l_matA = nullptr;
    float l_val;
    l_argReader.ReadObject(l_matA, g_matrix4Name);
    l_argReader.ReadNumber(l_val);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::mat4(*l_matA / l_val), g_matrix4Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Matrix4Defs::Multiply(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    
    if(l_argReader.IsNextNumber()) // Number * Matrix
    {
        float l_val;
        glm::mat4 *l_mat;
        l_argReader.ReadNumber(l_val);
        l_argReader.ReadObject(l_mat, g_matrix4Name);
        if(!l_argReader.HasError())
            l_argReader.PushObject(new glm::mat4(*l_mat * l_val), g_matrix4Name, false);
        else
            l_argReader.PushBoolean(false);
    }
    else
    {
        // Matrix * <something>
        glm::mat4 *l_mat;
        l_argReader.ReadObject(l_mat, g_matrix4Name);
        if(l_argReader.IsNextNumber()) // Matrix * number
        {
            float l_val;
            l_argReader.ReadNumber(l_val);
            if(!l_argReader.HasError())
                l_argReader.PushObject(new glm::mat4(*l_mat * l_val), g_matrix4Name, false);
            else
                l_argReader.PushBoolean(false);
        }
        else
        {
            // Matrix * Matrix
            glm::mat4 *l_matB = nullptr;
            l_argReader.ReadObject(l_matB, g_matrix4Name);
            if(!l_argReader.HasError())
                l_argReader.PushObject(new glm::mat4(*l_mat * *l_matB), g_matrix4Name, false);
            else
                l_argReader.PushBoolean(false);
        }
    }

    l_argReader.LogError();
    return 1;
}

int Matrix4Defs::GetDeterminant(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::mat4 *l_mat = nullptr;
    l_argReader.ReadObject(l_mat, g_matrix4Name);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(glm::determinant(*l_mat));
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int Matrix4Defs::GetInverse(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::mat4 *l_mat = nullptr;
    l_argReader.ReadObject(l_mat, g_matrix4Name);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::mat4(glm::inverse(*l_mat)), g_matrix4Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int Matrix4Defs::IsMatrix4(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::mat4 *l_mat = nullptr;
    l_argReader.ReadNextObject(l_mat, g_matrix4Name);
    l_argReader.PushBoolean(l_mat != nullptr);
    return l_argReader.GetReturnValue();
}

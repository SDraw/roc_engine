#include "stdafx.h"
#include "Lua/LuaDefs/Matrix2Defs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaArgReader.h"
#include "Utils.h"

extern const std::string g_matrix2Name("Matrix2");

std::vector<LuaPropDef> Matrix2Defs::ms_staticProps;
std::vector<LuaMethodDef> Matrix2Defs::ms_staticMethods;
std::vector<LuaMethodDef> Matrix2Defs::ms_metaMethods;
std::vector<LuaPropDef> Matrix2Defs::ms_instanceProps;
std::vector<LuaMethodDef> Matrix2Defs::ms_instanceMethods;

void Matrix2Defs::Init()
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

void Matrix2Defs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_matrix2Name, Create, &ms_staticProps, &ms_staticMethods, &ms_metaMethods, &ms_instanceProps, &ms_instanceMethods);
    p_vm->RegisterFunction("isMatrix2", IsMatrix2);
}

int Matrix2Defs::Create(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    float l_val[4] = { 0.f };
    l_argReader.Skip();
    for(size_t i = 0U; i < 9U; i++)
        l_argReader.ReadNextNumber(l_val[i]);
    l_argReader.PushObject(
        new glm::mat2(
            l_val[0], l_val[1],
            l_val[2], l_val[3]
        ), g_matrix2Name, false
    );
    return l_argReader.GetReturnValue();
}

int Matrix2Defs::GetIdentity(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::mat2(1.f), g_matrix2Name, false);
    return 1;
}
int Matrix2Defs::GetZero(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushObject(new glm::mat2(0.f), g_matrix2Name, false);
    return 1;
}

int Matrix2Defs::Add(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::mat2 *l_matA = nullptr;
    glm::mat2 *l_matB = nullptr;
    l_argReader.ReadObject(l_matA, g_matrix2Name);
    l_argReader.ReadObject(l_matB, g_matrix2Name);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::mat2(*l_matA + *l_matB), g_matrix2Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Matrix2Defs::Subtract(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::mat2 *l_matA = nullptr;
    glm::mat2 *l_matB = nullptr;
    l_argReader.ReadObject(l_matA, g_matrix2Name);
    l_argReader.ReadObject(l_matB, g_matrix2Name);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::mat2(*l_matA - *l_matB), g_matrix2Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Matrix2Defs::Divide(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::mat2 *l_matA = nullptr;
    float l_val;
    l_argReader.ReadObject(l_matA, g_matrix2Name);
    l_argReader.ReadNumber(l_val);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::mat2(*l_matA / l_val), g_matrix2Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int Matrix2Defs::Multiply(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    
    if(l_argReader.IsNextNumber()) // Number * Matrix
    {
        float l_val;
        glm::mat2 *l_mat;
        l_argReader.ReadNumber(l_val);
        l_argReader.ReadObject(l_mat, g_matrix2Name);
        if(!l_argReader.HasError())
            l_argReader.PushObject(new glm::mat2(*l_mat * l_val), g_matrix2Name, false);
        else
            l_argReader.PushBoolean(false);
    }
    else
    {
        // Matrix * <something>
        glm::mat2 *l_mat;
        l_argReader.ReadObject(l_mat, g_matrix2Name);
        if(l_argReader.IsNextNumber()) // Matrix * number
        {
            float l_val;
            l_argReader.ReadNumber(l_val);
            if(!l_argReader.HasError())
                l_argReader.PushObject(new glm::mat2(*l_mat * l_val), g_matrix2Name, false);
            else
                l_argReader.PushBoolean(false);
        }
        else
        {
            // Matrix * Matrix
            glm::mat2 *l_matB = nullptr;
            l_argReader.ReadObject(l_matB, g_matrix2Name);
            if(!l_argReader.HasError())
                l_argReader.PushObject(new glm::mat2(*l_mat * *l_matB), g_matrix2Name, false);
            else
                l_argReader.PushBoolean(false);
        }
    }

    l_argReader.LogError();
    return 1;
}

int Matrix2Defs::GetDeterminant(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::mat2 *l_mat = nullptr;
    l_argReader.ReadObject(l_mat, g_matrix2Name);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(glm::determinant(*l_mat));
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int Matrix2Defs::GetInverse(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::mat2 *l_mat = nullptr;
    l_argReader.ReadObject(l_mat, g_matrix2Name);
    if(!l_argReader.HasError())
        l_argReader.PushObject(new glm::mat2(glm::inverse(*l_mat)), g_matrix2Name, false);
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int Matrix2Defs::IsMatrix2(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::mat2 *l_mat = nullptr;
    l_argReader.ReadNextObject(l_mat, g_matrix2Name);
    l_argReader.PushBoolean(l_mat != nullptr);
    return l_argReader.GetReturnValue();
}

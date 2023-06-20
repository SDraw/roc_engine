#include "stdafx.h"
#include "Lua/LuaDefs/MeshDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaDefs/ElementDefs.h"
#include "Lua/LuaArgReader.h"
#include "Core.h"
#include "Utils.h"

extern const std::string g_meshName("Mesh");
const std::string g_meshInvalid("Invalid object/Mesh");

std::vector<LuaMethodDef> MeshDefs::ms_metaMethods;
std::vector<LuaPropDef> MeshDefs::ms_instanceProps;

void MeshDefs::Init()
{
    ms_instanceProps.emplace_back("boundSphereRadius", GetBoundSphereRadius, nullptr);
    ms_instanceProps.emplace_back("materialsCount", GetMaterialsCount, nullptr);

    ElementDefs::InheritTo(&ms_metaMethods, &ms_instanceProps);
}

void MeshDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_meshName, Create, nullptr, nullptr, &ms_metaMethods, &ms_instanceProps, nullptr);
    p_vm->RegisterFunction("isMesh", IsMesh);
}

int MeshDefs::Create(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    std::string l_path;
    l_argReader.Skip();
    l_argReader.ReadString(l_path);
    if(!l_argReader.HasError())
    {
        ROC::IMesh *l_mesh = Core::GetInstance()->GetEngineCore()->GetIElementManager()->CreateIMesh(l_path.c_str());
        if(l_mesh)
            l_argReader.PushObject(Utils::Cast<ROC::IElement*>(l_mesh), g_meshName, true);
        else
            l_argReader.PushBoolean(false);
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int MeshDefs::GetBoundSphereRadius(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_meshName);
    if(!l_argReader.HasError())
    {
        ROC::IMesh *l_mesh;
        if(Utils::IsValid(l_obj) && (l_mesh = Utils::Cast<ROC::IMesh*>(l_obj)))
            l_argReader.PushNumber(l_mesh->GetBoundSphereRadius());
        else
        {
            l_argReader.SetError(g_meshInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int MeshDefs::GetMaterialsCount(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_meshName);
    if(!l_argReader.HasError())
    {
        ROC::IMesh *l_mesh;
        if(Utils::IsValid(l_obj) && (l_mesh = Utils::Cast<ROC::IMesh*>(l_obj)))
            l_argReader.PushInteger(l_mesh->GetMaterialsCount());
        else
        {
            l_argReader.SetError(g_meshInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int MeshDefs::IsMesh(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj = nullptr;
    l_argReader.ReadNextObject(l_obj, g_meshName);
    l_argReader.PushBoolean(l_obj && Utils::IsValid(l_obj) && Utils::Cast<ROC::IMesh*>(l_obj));
    return l_argReader.GetReturnValue();
}

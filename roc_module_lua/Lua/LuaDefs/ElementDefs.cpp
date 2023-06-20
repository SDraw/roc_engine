#include "stdafx.h"
#include "Lua/LuaDefs/ElementDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaArgReader.h"
#include "Core.h"
#include "Utils.h"

const std::string g_invalid("Invalid object/Element");

std::vector<LuaMethodDef> ElementDefs::ms_staticMethods;
std::vector<LuaMethodDef> ElementDefs::ms_metaMethods;
std::vector<LuaPropDef> ElementDefs::ms_instanceProps;

void ElementDefs::Init()
{
    ms_staticMethods.emplace_back("destroy", Destroy);

    ms_metaMethods.emplace_back("__eq", Equals);

    ms_instanceProps.emplace_back("type", GetType, nullptr);
}

void ElementDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass("Element", nullptr, nullptr, &ms_staticMethods, &ms_metaMethods, &ms_instanceProps, nullptr);
    p_vm->RegisterFunction("isElement", IsElement);
}

void ElementDefs::InheritTo(std::vector<LuaMethodDef> *p_meta, std::vector<LuaPropDef> *p_instanceProps)
{
    if(p_meta)
        p_meta->insert(p_meta->end(), ms_metaMethods.begin(), ms_metaMethods.end());
    if(p_instanceProps)
        p_instanceProps->insert(p_instanceProps->end(), ms_instanceProps.begin(), ms_instanceProps.end());
}

int ElementDefs::Destroy(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj);
    if(!l_argReader.HasError())
    {
        if(Utils::IsValid(l_obj))
            l_argReader.PushBoolean(Core::GetInstance()->GetEngineCore()->GetIElementManager()->DestroyIElement(l_obj));
        else
        {
            l_argReader.SetError(g_invalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int ElementDefs::Equals(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_objA;
    ROC::IElement *l_objB;
    l_argReader.ReadObject(l_objA);
    l_argReader.ReadObject(l_objB);
    if(!l_argReader.HasError())
    {
        if(Utils::IsValid(l_objA) && Utils::IsValid(l_objB))
            l_argReader.PushBoolean(l_objA == l_objB);
        else
            l_argReader.PushBoolean(false);
    }
    else

        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int ElementDefs::GetType(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj);
    if(!l_argReader.HasError())
    {
        if(Utils::IsValid(l_obj))
            l_argReader.PushString(l_obj->GetElementTypeName());
        else
        {
            l_argReader.SetError(g_invalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int ElementDefs::IsElement(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj = nullptr;
    l_argReader.ReadNextObject(l_obj);
    l_argReader.PushBoolean(l_obj && Utils::IsValid(l_obj));
    return l_argReader.GetReturnValue();
}

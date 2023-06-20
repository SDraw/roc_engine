#include "stdafx.h"
#include "Lua/LuaDefs/CollidableDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaArgReader.h"
#include "Utils.h"

const std::string g_collidableInvalid("Invalid object/Collidable");

std::vector<LuaMethodDef> CollidableDefs::ms_instanceMethods;

void CollidableDefs::Init()
{
    ms_instanceMethods.emplace_back("setCollidableWith", SetCollidableWith);
}

void CollidableDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass("Collidable", nullptr, nullptr, nullptr, nullptr, nullptr, &ms_instanceMethods);
    p_vm->RegisterFunction("isCollidable", IsCollidable);
}

void CollidableDefs::InheritTo(std::vector<LuaMethodDef> *p_instanceMethods)
{
    if(p_instanceMethods)
        p_instanceMethods->insert(p_instanceMethods->end(), ms_instanceMethods.begin(), ms_instanceMethods.end());
}

int CollidableDefs::SetCollidableWith(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_objA;
    ROC::IElement *l_objB;
    bool l_state;
    l_argReader.ReadObject(l_objA);
    l_argReader.ReadObject(l_objB);
    l_argReader.ReadBoolean(l_state);
    if(!l_argReader.HasError())
    {
        ROC::ICollidable *l_colA;
        if(Utils::IsValid(l_objA) && (l_colA = Utils::Cast<ROC::ICollidable*>(l_objA)))
        {
            ROC::ICollidable *l_colB;
            if(Utils::IsValid(l_objB) && (l_colB = Utils::Cast<ROC::ICollidable*>(l_objB)))
                l_argReader.PushBoolean(l_colA->SetCollidableWith(l_colB, l_state));
            else
            {
                l_argReader.SetError(g_collidableInvalid);
                l_argReader.PushBoolean(false);
            }
        }
        else
        {
            l_argReader.SetError(g_collidableInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int CollidableDefs::IsCollidable(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadNextObject(l_obj, "");
    l_argReader.PushBoolean(l_obj && Utils::IsValid(l_obj) && Utils::Cast<ROC::ICollidable*>(l_obj));
    return l_argReader.GetReturnValue();
}


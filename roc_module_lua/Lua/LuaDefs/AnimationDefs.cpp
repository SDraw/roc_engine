#include "stdafx.h"
#include "Lua/LuaDefs/AnimationDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaDefs/ElementDefs.h"
#include "Lua/LuaArgReader.h"
#include "Core.h"
#include "Utils.h"

extern const std::string g_animationName("Animation");
const std::string g_animationInvalid("Invalid object/Animation");

std::vector<LuaMethodDef> AnimationDefs::ms_metaMethods;
std::vector<LuaPropDef> AnimationDefs::ms_instanceProps;

void AnimationDefs::Init()
{
    ms_instanceProps.emplace_back("bonesCount", GetBonesCount, nullptr);
    ms_instanceProps.emplace_back("duration", GetDuration, nullptr);

    ElementDefs::InheritTo(&ms_metaMethods, &ms_instanceProps);
}
void AnimationDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_animationName, Create, nullptr, nullptr, &ms_metaMethods, &ms_instanceProps, nullptr);
    p_vm->RegisterFunction("isAnimation", IsAnimation);
}

int AnimationDefs::Create(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    std::string l_path;
    l_argReader.Skip();
    l_argReader.ReadString(l_path);
    if(!l_argReader.HasError())
    {
        ROC::IAnimation *l_anim = Core::GetInstance()->GetEngineCore()->GetIElementManager()->CreateIAnimation(l_path.c_str());
        if(l_anim)
            l_argReader.PushObject(Utils::Cast<ROC::IElement*>(l_anim), g_animationName, true);
        else
            l_argReader.PushBoolean(false);
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int AnimationDefs::GetBonesCount(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_animationName);
    if(!l_argReader.HasError())
    {
        ROC::IAnimation *l_anim;
        if(Utils::IsValid(l_obj) && (l_anim = Utils::Cast<ROC::IAnimation*>(l_obj)))
            l_argReader.PushInteger(l_anim->GetBonesCount());
        else
        {
            l_argReader.SetError(g_animationInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int AnimationDefs::GetDuration(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_animationName);
    if(!l_argReader.HasError())
    {
        ROC::IAnimation *l_anim;
        if(Utils::IsValid(l_obj) && (l_anim = Utils::Cast<ROC::IAnimation*>(l_obj)))
            l_argReader.PushInteger(static_cast<std::int64_t>(l_anim->GetDuration()));
        else
        {
            l_argReader.SetError(g_animationInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int AnimationDefs::IsAnimation(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_animationName);
    l_argReader.PushBoolean(l_obj && Utils::IsValid(l_obj) && Utils::Cast<ROC::IAnimation*>(l_obj));
    return l_argReader.GetReturnValue();
}

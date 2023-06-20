#include "stdafx.h"
#include "Lua/LuaDefs/ModelDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaDefs/ElementDefs.h"
#include "Lua/LuaDefs/TransformableDefs.h"
#include "Lua/LuaDefs/CollidableDefs.h"
#include "Lua/LuaArgReader.h"
#include "Core.h"
#include "Utils.h"

extern const std::string g_animationName;
extern const std::string g_meshName;
extern const std::string g_modelName("Model");
const std::string g_modelInvalid("Invalid object/Model");
const std::string g_animationInvalid("Invalid object/Animation");

const std::vector<std::string> g_modelAnimationProperties
{
    "speed", "progress", "blendTime"
};

std::vector<LuaMethodDef> ModelDefs::ms_metaMethods;
std::vector<LuaPropDef> ModelDefs::ms_instanceProps;
std::vector<LuaMethodDef> ModelDefs::ms_instanceMethods;

void ModelDefs::Init()
{
    ms_instanceProps.emplace_back("mesh", GetMesh, nullptr);
    ms_instanceProps.emplace_back("boundSphereRadius", GetBoundSphereRadius, nullptr);
    ms_instanceProps.emplace_back("animation", GetAnimation, SetAnimation);
    ms_instanceProps.emplace_back("isVisible", GetIsVisible, nullptr);

    ms_instanceMethods.emplace_back("playAnimation", PlayAnimation);
    ms_instanceMethods.emplace_back("pauseAnimation", PauseAnimation);
    ms_instanceMethods.emplace_back("resetAnimation", ResetAnimation);
    ms_instanceMethods.emplace_back("getAnimationProperty", GetAnimationProperty);
    ms_instanceMethods.emplace_back("setAnimationProperty", SetAnimationProperty);

    ElementDefs::InheritTo(&ms_metaMethods, &ms_instanceProps);
    TransformableDefs::InheritTo(&ms_instanceProps);
    CollidableDefs::InheritTo(&ms_instanceMethods);
}

void ModelDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_modelName, Create, nullptr, nullptr, &ms_metaMethods, &ms_instanceProps, &ms_instanceMethods);
    p_vm->RegisterFunction("isModel", IsModel);
}

int ModelDefs::Create(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj = nullptr;
    l_argReader.Skip();
    l_argReader.ReadNextObject(l_obj, g_meshName);
    if(l_obj && Utils::IsValid(l_obj))
    {
        ROC::IModel *l_model = Core::GetInstance()->GetEngineCore()->GetIElementManager()->CreateIModel(Utils::Cast<ROC::IMesh*>(l_obj));
        if(l_model)
            l_argReader.PushObject(Utils::Cast<ROC::IElement*>(l_model), g_modelName, true);
        else
            l_argReader.PushBoolean(false);
    }
    else
    {
        ROC::IModel *l_model = Core::GetInstance()->GetEngineCore()->GetIElementManager()->CreateIModel(nullptr);
        if(l_model)
            l_argReader.PushObject(Utils::Cast<ROC::IElement*>(l_model), g_modelName, true);
        else
            l_argReader.PushBoolean(false);
    }

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int ModelDefs::GetMesh(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_modelName);
    if(!l_argReader.HasError())
    {
        ROC::IModel *l_model;
        if(Utils::IsValid(l_obj) && (l_model = Utils::Cast<ROC::IModel*>(l_obj)))
        {
            ROC::IMesh *l_mesh = l_model->GetIMesh();
            if(l_mesh)
                l_argReader.PushObject(Utils::Cast<ROC::IElement*>(l_mesh), g_meshName, true);
            else
                l_argReader.PushBoolean(false);
        }
        else
        {
            l_argReader.SetError(g_modelInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int ModelDefs::GetBoundSphereRadius(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_modelName);
    if(!l_argReader.HasError())
    {
        ROC::IModel *l_model;
        if(Utils::IsValid(l_obj) && (l_model = Utils::Cast<ROC::IModel*>(l_obj)))
            l_argReader.PushNumber(l_model->GetBoundSphereRadius());
        else
        {
            l_argReader.SetError(g_modelInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int ModelDefs::GetAnimation(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_modelName);
    if(!l_argReader.HasError())
    {
        ROC::IModel *l_model;
        if(Utils::IsValid(l_obj) && (l_model = Utils::Cast<ROC::IModel*>(l_obj)))
        {
            ROC::IAnimation *l_anim = l_model->GetIAnimation();
            if(l_anim)
                l_argReader.PushObject(Utils::Cast<ROC::IElement*>(l_anim), g_animationName, true);
            else
                l_argReader.PushBoolean(false);
        }
        else
        {
            l_argReader.SetError(g_modelInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.PushBoolean(false);
    return 1;
}
int ModelDefs::SetAnimation(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_modelObj;
    ROC::IElement *l_animObj = nullptr;
    l_argReader.ReadObject(l_modelObj, g_modelName);
    l_argReader.ReadNextObject(l_animObj, g_animationName);
    if(!l_argReader.HasError())
    {
        ROC::IModel *l_model;
        if(Utils::IsValid(l_modelObj) && (l_model = Utils::Cast<ROC::IModel*>(l_modelObj)))
        {
            ROC::IAnimation *l_anim;
            if(l_animObj && Utils::IsValid(l_animObj) && (l_anim = Utils::Cast<ROC::IAnimation*>(l_animObj)))
                l_argReader.PushBoolean(l_model->SetIAnimation(l_anim));
            else
                l_argReader.PushBoolean(l_model->RemoveAnimation());
        }
        else
            l_argReader.SetError(g_modelInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int ModelDefs::GetIsVisible(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_modelName);
    if(!l_argReader.HasError())
    {
        ROC::IModel *l_model;
        if(Utils::IsValid(l_obj) && (l_model = Utils::Cast<ROC::IModel*>(l_obj)))
            l_argReader.PushBoolean(l_model->IsVisible());
        else
        {
            l_argReader.SetError(g_modelInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int ModelDefs::PlayAnimation(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    bool l_loop = true;
    l_argReader.ReadObject(l_obj, g_modelName);
    l_argReader.ReadNextBoolean(l_loop);
    if(!l_argReader.HasError())
    {
        ROC::IModel *l_model;
        if(Utils::IsValid(l_obj) && (l_model = Utils::Cast<ROC::IModel*>(l_obj)))
            l_argReader.PushBoolean(l_model->PlayAnimation(l_loop));
        else
        {
            l_argReader.SetError(g_modelInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int ModelDefs::PauseAnimation(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_modelName);
    if(!l_argReader.HasError())
    {
        ROC::IModel *l_model;
        if(Utils::IsValid(l_obj) && (l_model = Utils::Cast<ROC::IModel*>(l_obj)))
            l_argReader.PushBoolean(l_model->PauseAnimation());
        else
        {
            l_argReader.SetError(g_modelInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int ModelDefs::ResetAnimation(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_modelName);
    if(!l_argReader.HasError())
    {
        ROC::IModel *l_model;
        if(Utils::IsValid(l_obj) && (l_model = Utils::Cast<ROC::IModel*>(l_obj)))
            l_argReader.PushBoolean(l_model->ResetAnimation());
        else
        {
            l_argReader.SetError(g_modelInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int ModelDefs::GetAnimationProperty(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    ROC::IModel::ModelAnimationProperty l_prop;
    l_argReader.ReadObject(l_obj, g_modelName);
    l_argReader.ReadEnum(l_prop, g_modelAnimationProperties);
    if(!l_argReader.HasError())
    {
        ROC::IModel *l_model;
        if(Utils::IsValid(l_obj) && (l_model = Utils::Cast<ROC::IModel*>(l_obj)))
        {
            float l_val;
            if(l_model->GetAnimationProperty(l_prop, l_val))
                l_argReader.PushNumber(l_val);
            else
                l_argReader.PushBoolean(false);
        }
        else
        {
            l_argReader.SetError(g_modelInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int ModelDefs::SetAnimationProperty(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    ROC::IModel::ModelAnimationProperty l_prop;
    float l_val;
    l_argReader.ReadObject(l_obj, g_modelName);
    l_argReader.ReadEnum(l_prop, g_modelAnimationProperties);
    l_argReader.ReadNumber(l_val);
    if(!l_argReader.HasError())
    {
        ROC::IModel *l_model;
        if(Utils::IsValid(l_obj) && (l_model = Utils::Cast<ROC::IModel*>(l_obj)))
            l_argReader.PushBoolean(l_model->SetAnimationProperty(l_prop, l_val));
        else
        {
            l_argReader.SetError(g_modelInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int ModelDefs::IsModel(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj = nullptr;
    l_argReader.ReadNextObject(l_obj, g_modelName);
    l_argReader.PushBoolean(l_obj && Utils::IsValid(l_obj) && Utils::Cast<ROC::IModel*>(l_obj));
    return l_argReader.GetReturnValue();
}

#include "stdafx.h"
#include "Lua/LuaDefs/TransformableDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaArgReader.h"
#include "Utils.h"

extern const std::string g_vec3Name;
extern const std::string g_quatName;
extern const std::string g_matrixName;

const std::string g_transformableInvalid("Invalid object/Transformable");

std::vector<LuaPropDef> TransformableDefs::ms_instanceProps;

void TransformableDefs::Init()
{
    ms_instanceProps.emplace_back("position", GetPosition, SetPosition);
    ms_instanceProps.emplace_back("rotation", GetRotation, SetRotation);
    ms_instanceProps.emplace_back("scale", GetScale, SetScale);
    ms_instanceProps.emplace_back("matrix", GetMatrix, nullptr);
}

void TransformableDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass("Transformable", nullptr, nullptr, nullptr, nullptr, &ms_instanceProps, nullptr);
    p_vm->RegisterFunction("isTransformable", IsTransformable);
}

void TransformableDefs::InheritTo(std::vector<LuaPropDef> *p_instanceProps)
{
    if(p_instanceProps)
        p_instanceProps->insert(p_instanceProps->end(), ms_instanceProps.begin(), ms_instanceProps.end());
}

int TransformableDefs::GetPosition(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj);
    if(!l_argReader.HasError())
    {
        ROC::ITransformable *l_transform;
        if(Utils::IsValid(l_obj) && (l_transform = Utils::Cast<ROC::ITransformable*>(l_obj)))
          l_argReader.PushObject(new glm::vec3(l_transform->GetPosition()), g_vec3Name, false);
        else
        {
            l_argReader.SetError(g_transformableInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int TransformableDefs::SetPosition(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec3 *l_vec;
    l_argReader.ReadObject(l_obj);
    l_argReader.ReadObject(l_vec, g_vec3Name);
    if(!l_argReader.HasError())
    {
        ROC::ITransformable *l_transform;
        if(Utils::IsValid(l_obj) && (l_transform = Utils::Cast<ROC::ITransformable*>(l_obj)))
            l_transform->SetPosition(*l_vec);
        else
            l_argReader.SetError(g_transformableInvalid);
    }

    l_argReader.LogError();
    return 0;
}
int TransformableDefs::GetRotation(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj);
    if(!l_argReader.HasError())
    {
        ROC::ITransformable *l_transform;
        if(Utils::IsValid(l_obj) && (l_transform = Utils::Cast<ROC::ITransformable*>(l_obj)))
            l_argReader.PushObject(new glm::quat(l_transform->GetRotation()), g_quatName, false);
        else
        {
            l_argReader.SetError(g_transformableInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int TransformableDefs::SetRotation(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::quat *l_quat;
    l_argReader.ReadObject(l_obj);
    l_argReader.ReadObject(l_quat, g_quatName);
    if(!l_argReader.HasError())
    {
        ROC::ITransformable *l_transform;
        if(Utils::IsValid(l_obj) && (l_transform = Utils::Cast<ROC::ITransformable*>(l_obj)))
            l_transform->SetRotation(*l_quat);
        else
            l_argReader.SetError(g_transformableInvalid);
    }

    l_argReader.LogError();
    return 0;
}
int TransformableDefs::GetScale(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj);
    if(!l_argReader.HasError())
    {
        ROC::ITransformable *l_transform;
        if(Utils::IsValid(l_obj) && (l_transform = Utils::Cast<ROC::ITransformable*>(l_obj)))
            l_argReader.PushObject(new glm::vec3(l_transform->GetScale()), g_vec3Name, false);
        else
        {
            l_argReader.SetError(g_transformableInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int TransformableDefs::SetScale(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec3 *l_vec;
    l_argReader.ReadObject(l_obj);
    l_argReader.ReadObject(l_vec, g_vec3Name);
    if(!l_argReader.HasError())
    {
        ROC::ITransformable *l_transform;
        if(Utils::IsValid(l_obj) && (l_transform = Utils::Cast<ROC::ITransformable*>(l_obj)))
            l_transform->SetPosition(*l_vec);
        else
            l_argReader.SetError(g_transformableInvalid);
    }

    l_argReader.LogError();
    return 0;
}
int TransformableDefs::GetMatrix(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj);
    if(!l_argReader.HasError())
    {
        ROC::ITransformable *l_transform;
        if(Utils::IsValid(l_obj) && (l_transform = Utils::Cast<ROC::ITransformable*>(l_obj)))
            l_argReader.PushObject(new glm::mat4x4(l_transform->GetMatrix()), g_matrixName, false);
        else
        {
            l_argReader.SetError(g_transformableInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}


int TransformableDefs::IsTransformable(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadNextObject(l_obj, "");
    l_argReader.PushBoolean(l_obj && Utils::IsValid(l_obj) && Utils::Cast<ROC::ITransformable*>(l_obj));
    return l_argReader.GetReturnValue();
}


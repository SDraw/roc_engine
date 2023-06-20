#include "stdafx.h"
#include "Lua/LuaDefs/LightDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaDefs/ElementDefs.h"
#include "Lua/LuaDefs/TransformableDefs.h"
#include "Lua/LuaArgReader.h"
#include "Core.h"
#include "Utils.h"

extern const std::string g_vec2Name;
extern const std::string g_vec3Name;
extern const std::string g_vec4Name;
extern const std::string g_lightName("Light");
extern const std::string g_lightInvalid("Invalid object/Light");

const std::vector<std::string> g_lightTypes
{
  "directional", "point", "spotlight"
};

std::vector<LuaMethodDef> LightDefs::ms_metaMethods;
std::vector<LuaPropDef> LightDefs::ms_instanceProps;

void LightDefs::Init()
{
    ms_instanceProps.emplace_back("lightType", GetType, SetType);
    ms_instanceProps.emplace_back("direction", GetDirection, SetDirection);
    ms_instanceProps.emplace_back("color", GetColor, SetColor);
    ms_instanceProps.emplace_back("cutoff", GetCutoff, SetCutoff);
    ms_instanceProps.emplace_back("falloff", GetFalloff, SetFalloff);

    ElementDefs::InheritTo(&ms_metaMethods, &ms_instanceProps);
    TransformableDefs::InheritTo(&ms_instanceProps);
}
void LightDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_lightName, Create, nullptr, nullptr, &ms_metaMethods, &ms_instanceProps, nullptr);
    p_vm->RegisterFunction("isLight", IsLight);
}

int LightDefs::Create(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::ILight::LightType l_type;
    l_argReader.Skip();
    l_argReader.ReadEnum(l_type, g_lightTypes);
    if(!l_argReader.HasError())
    {
        ROC::ILight *l_light = Core::GetInstance()->GetEngineCore()->GetIElementManager()->CreateILight(l_type);
        if(l_light)
            l_argReader.PushObject(Utils::Cast<ROC::IElement*>(l_light), g_lightName, true);
        else
            l_argReader.PushBoolean(false);
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int LightDefs::GetType(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_lightName);
    if(!l_argReader.HasError())
    {
        ROC::ILight *l_light = nullptr;
        if(Utils::IsValid(l_obj) && (l_light = Utils::Cast<ROC::ILight*>(l_obj)))
            l_argReader.PushString(g_lightTypes[l_light->GetType()]);
        else
        {
            l_argReader.SetError(g_lightInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int LightDefs::SetType(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    ROC::ILight::LightType l_type;
    l_argReader.ReadObject(l_obj, g_lightName);
    l_argReader.ReadEnum(l_type, g_lightTypes);
    if(!l_argReader.HasError())
    {
        ROC::ILight *l_light;
        if(Utils::IsValid(l_obj) && (l_light = Utils::Cast<ROC::ILight*>(l_obj)))
            l_light->SetType(l_type);
        else
            l_argReader.SetError(g_lightInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int LightDefs::GetDirection(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_lightName);
    if(!l_argReader.HasError())
    {
        ROC::ILight *l_light = nullptr;
        if(Utils::IsValid(l_obj) && (l_light = Utils::Cast<ROC::ILight*>(l_obj)))
            l_argReader.PushObject(new glm::vec3(l_light->GetDirection()), g_vec3Name, false);
        else
        {
            l_argReader.SetError(g_lightInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int LightDefs::SetDirection(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec3 *l_vec;
    l_argReader.ReadObject(l_obj, g_lightName);
    l_argReader.ReadObject(l_vec, g_vec3Name);
    if(!l_argReader.HasError())
    {
        ROC::ILight *l_light;
        if(Utils::IsValid(l_obj) && (l_light = Utils::Cast<ROC::ILight*>(l_obj)))
            l_light->SetDirection(*l_vec);
        else
            l_argReader.SetError(g_lightInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int LightDefs::GetColor(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_lightName);
    if(!l_argReader.HasError())
    {
        ROC::ILight *l_light = nullptr;
        if(Utils::IsValid(l_obj) && (l_light = Utils::Cast<ROC::ILight*>(l_obj)))
            l_argReader.PushObject(new glm::vec4(l_light->GetColor()), g_vec4Name, false);
        else
        {
            l_argReader.SetError(g_lightInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int LightDefs::SetColor(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec4 *l_vec;
    l_argReader.ReadObject(l_obj, g_lightName);
    l_argReader.ReadObject(l_vec, g_vec4Name);
    if(!l_argReader.HasError())
    {
        ROC::ILight *l_light;
        if(Utils::IsValid(l_obj) && (l_light = Utils::Cast<ROC::ILight*>(l_obj)))
            l_light->SetColor(*l_vec);
        else
            l_argReader.SetError(g_lightInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int LightDefs::GetCutoff(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_lightName);
    if(!l_argReader.HasError())
    {
        ROC::ILight *l_light = nullptr;
        if(Utils::IsValid(l_obj) && (l_light = Utils::Cast<ROC::ILight*>(l_obj)))
            l_argReader.PushObject(new glm::vec2(l_light->GetCutoff()), g_vec2Name, false);
        else
        {
            l_argReader.SetError(g_lightInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int LightDefs::SetCutoff(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec2 *l_vec;
    l_argReader.ReadObject(l_obj, g_lightName);
    l_argReader.ReadObject(l_vec, g_vec2Name);
    if(!l_argReader.HasError())
    {
        ROC::ILight *l_light;
        if(Utils::IsValid(l_obj) && (l_light = Utils::Cast<ROC::ILight*>(l_obj)))
            l_light->SetCutoff(*l_vec);
        else
            l_argReader.SetError(g_lightInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int LightDefs::GetFalloff(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_lightName);
    if(!l_argReader.HasError())
    {
        ROC::ILight *l_light = nullptr;
        if(Utils::IsValid(l_obj) && (l_light = Utils::Cast<ROC::ILight*>(l_obj)))
            l_argReader.PushObject(new glm::vec3(l_light->GetFalloff()), g_vec3Name, false);
        else
        {
            l_argReader.SetError(g_lightInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int LightDefs::SetFalloff(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec3 *l_vec;
    l_argReader.ReadObject(l_obj, g_lightName);
    l_argReader.ReadObject(l_vec, g_vec3Name);
    if(!l_argReader.HasError())
    {
        ROC::ILight *l_light;
        if(Utils::IsValid(l_obj) && (l_light = Utils::Cast<ROC::ILight*>(l_obj)))
            l_light->SetFalloff(*l_vec);
        else
            l_argReader.SetError(g_lightInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int LightDefs::IsLight(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj = nullptr;
    l_argReader.ReadNextObject(l_obj, g_lightName);
    l_argReader.PushBoolean(l_obj && Utils::IsValid(l_obj) && Utils::Cast<ROC::ILight*>(l_obj));
    return l_argReader.GetReturnValue();
}

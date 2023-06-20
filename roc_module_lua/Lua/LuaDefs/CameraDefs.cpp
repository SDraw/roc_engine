#include "stdafx.h"
#include "Lua/LuaDefs/CameraDefs.h"
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
extern const std::string g_matrixName;
extern const std::string g_cameraName("Camera");
const std::string g_cameraInvalid("Invalid object/Camera");

const std::vector<std::string> g_projectionTypes
{
    "perspective", "orthogonal", "screen"
};

std::vector<LuaMethodDef> CameraDefs::ms_metaMethods;
std::vector<LuaPropDef> CameraDefs::ms_instanceProps;

void CameraDefs::Init()
{
    ms_instanceProps.emplace_back("projectionType", GetProjectionType, SetProjectionType);
    ms_instanceProps.emplace_back("fov", GetFov, SetFov);
    ms_instanceProps.emplace_back("aspectRatio", GetAspectRatio, SetAspectRatio);
    ms_instanceProps.emplace_back("orthoParams", GetOrthoParams, SetOrthoParams);
    ms_instanceProps.emplace_back("depth", GetDepth, SetDepth);
    ms_instanceProps.emplace_back("direction", GetDirection, SetDirection);
    ms_instanceProps.emplace_back("up", GetUp, SetUp);
    ms_instanceProps.emplace_back("viewMatrix", GetViewMatrix, nullptr);
    ms_instanceProps.emplace_back("projectionMatrix", GetProjectionMatrix, nullptr);
    ms_instanceProps.emplace_back("viewProjectionMatrix", GetViewProjectionMatrix, nullptr);

    ElementDefs::InheritTo(&ms_metaMethods, &ms_instanceProps);
    TransformableDefs::InheritTo(&ms_instanceProps);
}

void CameraDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_cameraName, Create, nullptr, nullptr, &ms_metaMethods, &ms_instanceProps, nullptr);
    p_vm->RegisterFunction("isCamera", IsCamera);
}

int CameraDefs::Create(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::ICamera::CameraProjectionType l_projType;
    l_argReader.Skip();
    l_argReader.ReadEnum(l_projType, g_projectionTypes);
    if(!l_argReader.HasError())
    {
        ROC::ICamera *l_cam = Core::GetInstance()->GetEngineCore()->GetIElementManager()->CreateICamera(l_projType);
        if(l_cam)
            l_argReader.PushObject(Utils::Cast<ROC::IElement*>(l_cam), g_cameraName, true);
        else
            l_argReader.PushBoolean(false);
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int CameraDefs::GetProjectionType(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_cameraName);
    if(!l_argReader.HasError())
    {
        ROC::ICamera *l_camera = nullptr;
        if(Utils::IsValid(l_obj) && (l_camera = Utils::Cast<ROC::ICamera*>(l_obj)))
            l_argReader.PushString(g_projectionTypes[l_camera->GetProjectionType()]);
        else
        {
            l_argReader.SetError(g_cameraInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int CameraDefs::SetProjectionType(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    ROC::ICamera::CameraProjectionType l_projType;
    l_argReader.ReadObject(l_obj, g_cameraName);
    l_argReader.ReadEnum(l_projType, g_projectionTypes);
    if(!l_argReader.HasError())
    {
        ROC::ICamera *l_camera;
        if(Utils::IsValid(l_obj) && (l_camera = Utils::Cast<ROC::ICamera*>(l_obj)))
            l_camera->SetProjectionType(l_projType);
        else
            l_argReader.SetError(g_cameraInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int CameraDefs::GetFov(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_cameraName);
    if(!l_argReader.HasError())
    {
        ROC::ICamera *l_camera;
        if(Utils::IsValid(l_obj) && (l_camera = Utils::Cast<ROC::ICamera*>(l_obj)))
            l_argReader.PushNumber(l_camera->GetFOV());
        else
        {
            l_argReader.SetError(g_cameraInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int CameraDefs::SetFov(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    float l_value;
    l_argReader.ReadObject(l_obj, g_cameraName);
    l_argReader.ReadNumber(l_value);
    if(!l_argReader.HasError())
    {
        ROC::ICamera *l_camera;
        if(Utils::IsValid(l_obj) && (l_camera = Utils::Cast<ROC::ICamera*>(l_obj)))
            l_camera->SetFOV(l_value);
        else
            l_argReader.SetError(g_cameraInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int CameraDefs::GetAspectRatio(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_cameraName);
    if(!l_argReader.HasError())
    {
        ROC::ICamera *l_camera;
        if(Utils::IsValid(l_obj) && (l_camera = Utils::Cast<ROC::ICamera*>(l_obj)))
            l_argReader.PushNumber(l_camera->GetAspectRatio());
        else
        {
            l_argReader.SetError(g_cameraInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int CameraDefs::SetAspectRatio(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    float l_value;
    l_argReader.ReadObject(l_obj, g_cameraName);
    l_argReader.ReadNumber(l_value);
    if(!l_argReader.HasError())
    {
        ROC::ICamera *l_camera;
        if(Utils::IsValid(l_obj) && (l_camera = Utils::Cast<ROC::ICamera*>(l_obj)))
            l_camera->SetAspectRatio(l_value);
        else
            l_argReader.SetError(g_cameraInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int CameraDefs::GetOrthoParams(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_cameraName);
    if(!l_argReader.HasError())
    {
        ROC::ICamera *l_camera;
        if(Utils::IsValid(l_obj) && (l_camera = Utils::Cast<ROC::ICamera*>(l_obj)))
            l_argReader.PushObject(new glm::vec4(l_camera->GetOrthoParams()), g_vec4Name, false);
        else
        {
            l_argReader.SetError(g_cameraInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int CameraDefs::SetOrthoParams(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec4 *l_value;
    l_argReader.ReadObject(l_obj, g_cameraName);
    l_argReader.ReadObject(l_value, g_vec4Name);
    if(!l_argReader.HasError())
    {
        ROC::ICamera *l_camera;
        if(Utils::IsValid(l_obj) && (l_camera = Utils::Cast<ROC::ICamera*>(l_obj)))
            l_camera->SetOrthoParams(*l_value);
        else
            l_argReader.SetError(g_cameraInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int CameraDefs::GetDepth(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_cameraName);
    if(!l_argReader.HasError())
    {
        ROC::ICamera *l_camera;
        if(Utils::IsValid(l_obj) && (l_camera = Utils::Cast<ROC::ICamera*>(l_obj)))
            l_argReader.PushObject(new glm::vec2(l_camera->GetDepth()), g_vec2Name, false);
        else
        {
            l_argReader.SetError(g_cameraInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int CameraDefs::SetDepth(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec2 *l_value;
    l_argReader.ReadObject(l_obj, g_cameraName);
    l_argReader.ReadObject(l_value, g_vec2Name);
    if(!l_argReader.HasError())
    {
        ROC::ICamera *l_camera;
        if(Utils::IsValid(l_obj) && (l_camera = Utils::Cast<ROC::ICamera*>(l_obj)))
            l_camera->SetDepth(*l_value);
        else
            l_argReader.SetError(g_cameraInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int CameraDefs::GetDirection(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_cameraName);
    if(!l_argReader.HasError())
    {
        ROC::ICamera *l_camera;
        if(Utils::IsValid(l_obj) && (l_camera = Utils::Cast<ROC::ICamera*>(l_obj)))
            l_argReader.PushObject(new glm::vec3(l_camera->GetDirection()), g_vec3Name, false);
        else
        {
            l_argReader.SetError(g_cameraInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int CameraDefs::SetDirection(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec3 *l_value;
    l_argReader.ReadObject(l_obj, g_cameraName);
    l_argReader.ReadObject(l_value, g_vec3Name);
    if(!l_argReader.HasError())
    {
        ROC::ICamera *l_camera;
        if(Utils::IsValid(l_obj) && (l_camera = Utils::Cast<ROC::ICamera*>(l_obj)))
            l_camera->SetDirection(*l_value);
        else
            l_argReader.SetError(g_cameraInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int CameraDefs::GetUp(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_cameraName);
    if(!l_argReader.HasError())
    {
        ROC::ICamera *l_camera;
        if(Utils::IsValid(l_obj) && (l_camera = Utils::Cast<ROC::ICamera*>(l_obj)))
            l_argReader.PushObject(new glm::vec3(l_camera->GetUpDirection()), g_vec3Name, false);
        else
        {
            l_argReader.SetError(g_cameraInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int CameraDefs::SetUp(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec3 *l_value;
    l_argReader.ReadObject(l_obj, g_cameraName);
    l_argReader.ReadObject(l_value, g_vec3Name);
    if(!l_argReader.HasError())
    {
        ROC::ICamera *l_camera;
        if(Utils::IsValid(l_obj) && (l_camera = Utils::Cast<ROC::ICamera*>(l_obj)))
            l_camera->SetUpDirection(*l_value);
        else
            l_argReader.SetError(g_cameraInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int CameraDefs::GetViewMatrix(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_cameraName);
    if(!l_argReader.HasError())
    {
        ROC::ICamera *l_camera;
        if(Utils::IsValid(l_obj) && (l_camera = Utils::Cast<ROC::ICamera*>(l_obj)))
            l_argReader.PushObject(new glm::mat4(l_camera->GetViewMatrix()), g_matrixName, false);
        else
        {
            l_argReader.SetError(g_cameraInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int CameraDefs::GetProjectionMatrix(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_cameraName);
    if(!l_argReader.HasError())
    {
        ROC::ICamera *l_camera;
        if(Utils::IsValid(l_obj) && (l_camera = Utils::Cast<ROC::ICamera*>(l_obj)))
            l_argReader.PushObject(new glm::mat4(l_camera->GetProjectionMatrix()), g_matrixName, false);
        else
        {
            l_argReader.SetError(g_cameraInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int CameraDefs::GetViewProjectionMatrix(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_cameraName);
    if(!l_argReader.HasError())
    {
        ROC::ICamera *l_camera;
        if(Utils::IsValid(l_obj) && (l_camera = Utils::Cast<ROC::ICamera*>(l_obj)))
            l_argReader.PushObject(new glm::mat4(l_camera->GetViewProjectionMatrix()), g_matrixName, false);
        else
        {
            l_argReader.SetError(g_cameraInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int CameraDefs::IsCamera(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_cameraName);
    l_argReader.PushBoolean(l_obj && Utils::IsValid(l_obj) && Utils::Cast<ROC::ICamera*>(l_obj));
    return l_argReader.GetReturnValue();
}

#include "stdafx.h"
#include "Lua/LuaDefs/SceneDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaArgReader.h"
#include "Lua/LuaDefs/ElementDefs.h"
#include "Core.h"
#include "Utils.h"

extern const std::string g_cameraName;
extern const std::string g_lightName;
extern const std::string g_modelName;
extern const std::string g_renderTargetName;
extern const std::string g_shaderName;
extern const std::string g_vec2Name;
const std::string g_sceneName("Scene");
const std::string g_sceneInvalid("Invalid object/Scene");
const std::string g_lightInvalid("Invalid object/Light");
const std::string g_modelInvalid("Invalid object/Model");

std::vector<LuaMethodDef> SceneDefs::ms_metaMethods;
std::vector<LuaPropDef> SceneDefs::ms_instanceProps;
std::vector<LuaMethodDef> SceneDefs::ms_instanceMethods;

void SceneDefs::Init()
{
    ms_instanceProps.emplace_back("camera", GetCamera, SetCamera);
    ms_instanceProps.emplace_back("renderTarget", GetRenderTarget, SetRenderTarget);
    ms_instanceProps.emplace_back("shader", GetShader, SetShader);
    ms_instanceProps.emplace_back("shadows", GetShadows, SetShadows);
    ms_instanceProps.emplace_back("shadowsArea", GetShadowsArea, SetShadowsArea);
    ms_instanceProps.emplace_back("shadowsDepth", GetShadowsDepth, SetShadowsDepth);
    ms_instanceProps.emplace_back("shadowsQuality", GetShadowsQuality, SetShadowsQuality);

    ms_instanceMethods.emplace_back("addLight", AddLight);
    ms_instanceMethods.emplace_back("removeLight", RemoveLight);
    ms_instanceMethods.emplace_back("hasLight", HasLight);
    ms_instanceMethods.emplace_back("addModel", AddModel);
    ms_instanceMethods.emplace_back("removeModel", RemoveModel);
    ms_instanceMethods.emplace_back("hasModel", HasModel);
    ms_instanceMethods.emplace_back("draw", Draw);

    ElementDefs::InheritTo(&ms_metaMethods, &ms_instanceProps);
}

void SceneDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_sceneName, Create, nullptr, nullptr, &ms_metaMethods, &ms_instanceProps, &ms_instanceMethods);
    p_vm->RegisterFunction("isScene", IsScene);
}

int SceneDefs::Create(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IScene *l_scene = Core::GetInstance()->GetEngineCore()->GetIElementManager()->CreateIScene();
    if(l_scene)
        l_argReader.PushObject(Utils::Cast<ROC::IElement*>(l_scene), g_sceneName, true);
    else
        l_argReader.PushBoolean(false);
    return l_argReader.GetReturnValue();
}

int SceneDefs::GetCamera(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_sceneName);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_obj) && (l_scene = Utils::Cast<ROC::IScene*>(l_obj)))
        {
            ROC::ICamera *l_camera = l_scene->GetICamera();
            if(l_camera)
                l_argReader.PushObject(Utils::Cast<ROC::IElement*>(l_camera), g_cameraName, true);
            else
                l_argReader.PushBoolean(false);
        }
        else
        {
            l_argReader.SetError(g_sceneInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int SceneDefs::SetCamera(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    ROC::IElement *l_camObj = nullptr;
    l_argReader.ReadObject(l_obj, g_sceneName);
    l_argReader.ReadNextObject(l_camObj, g_cameraName);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_obj) && (l_scene = Utils::Cast<ROC::IScene*>(l_obj)))
        {
            ROC::ICamera *l_camera;
            if(l_camObj && Utils::IsValid(l_camObj) && (l_camera = Utils::Cast<ROC::ICamera*>(l_camObj)))
                l_scene->SetICamera(l_camera);
            else
                l_scene->RemoveCamera();
        }
    }

    l_argReader.LogError();
    return 0;
}

int SceneDefs::GetRenderTarget(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_sceneName);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_obj) && (l_scene = Utils::Cast<ROC::IScene*>(l_obj)))
        {
            ROC::IRenderTarget *l_target = l_scene->GetIRenderTarget();
            if(l_target)
                l_argReader.PushObject(Utils::Cast<ROC::IElement*>(l_target), g_renderTargetName, true);
            else
                l_argReader.PushBoolean(false);
        }
        else
        {
            l_argReader.SetError(g_sceneInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int SceneDefs::SetRenderTarget(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    ROC::IElement *l_rtObj = nullptr;
    l_argReader.ReadObject(l_obj, g_sceneName);
    l_argReader.ReadNextObject(l_rtObj, g_renderTargetName);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_obj) && (l_scene = Utils::Cast<ROC::IScene*>(l_obj)))
        {
            ROC::IRenderTarget *l_target;
            if(l_rtObj && Utils::IsValid(l_rtObj) && (l_target = Utils::Cast<ROC::IRenderTarget*>(l_rtObj)))
                l_scene->SetIRenderTarget(l_target);
            else
                l_scene->RemoveRenderTarget();
        }
    }

    l_argReader.LogError();
    return 0;
}

int SceneDefs::GetShader(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_sceneName);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_obj) && (l_scene = Utils::Cast<ROC::IScene*>(l_obj)))
        {
            ROC::IShader *l_shader = l_scene->GetIShader();
            if(l_shader)
                l_argReader.PushObject(Utils::Cast<ROC::IElement*>(l_shader), g_shaderName, true);
            else
                l_argReader.PushBoolean(false);
        }
        else
        {
            l_argReader.SetError(g_sceneInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int SceneDefs::SetShader(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    ROC::IElement *l_shaderObj = nullptr;
    l_argReader.ReadObject(l_obj, g_sceneName);
    l_argReader.ReadNextObject(l_shaderObj, g_shaderName);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_obj) && (l_scene = Utils::Cast<ROC::IScene*>(l_obj)))
        {
            ROC::IShader *l_shader;
            if(l_shaderObj && Utils::IsValid(l_shaderObj) && (l_shader = Utils::Cast<ROC::IShader*>(l_shaderObj)))
                l_scene->SetIShader(l_shader);
            else
                l_scene->RemoveIShader();
        }
    }

    l_argReader.LogError();
    return 0;
}

int SceneDefs::GetShadows(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_sceneName);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_obj) && (l_scene = Utils::Cast<ROC::IScene*>(l_obj)))
            l_argReader.PushBoolean(l_scene->HasShadows());
        else
        {
            l_argReader.SetError(g_sceneInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int SceneDefs::SetShadows(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    bool l_val;
    l_argReader.ReadObject(l_obj, g_sceneName);
    l_argReader.ReadBoolean(l_val);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_obj) && (l_scene = Utils::Cast<ROC::IScene*>(l_obj)))
            l_scene->SetShadows(l_val);
    }

    l_argReader.LogError();
    return 0;
}

int SceneDefs::GetShadowsArea(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_sceneName);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_obj) && (l_scene = Utils::Cast<ROC::IScene*>(l_obj)))
        {
            glm::vec2 l_area;
            l_scene->GetShadowsArea(l_area);
            l_argReader.PushObject(new glm::vec2(l_area), g_vec2Name, false);
        }
        else
        {
            l_argReader.SetError(g_sceneInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int SceneDefs::SetShadowsArea(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec2 *l_val;
    l_argReader.ReadObject(l_obj, g_sceneName);
    l_argReader.ReadObject(l_val, g_vec2Name);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_obj) && (l_scene = Utils::Cast<ROC::IScene*>(l_obj)))
            l_scene->SetShadowsArea(*l_val);
    }

    l_argReader.LogError();
    return 0;
}

int SceneDefs::GetShadowsDepth(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_sceneName);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_obj) && (l_scene = Utils::Cast<ROC::IScene*>(l_obj)))
        {
            glm::vec2 l_area;
            l_scene->GetShadowsDepth(l_area);
            l_argReader.PushObject(new glm::vec2(l_area), g_vec2Name, false);
        }
        else
        {
            l_argReader.SetError(g_sceneInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int SceneDefs::SetShadowsDepth(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec2 *l_val;
    l_argReader.ReadObject(l_obj, g_sceneName);
    l_argReader.ReadObject(l_val, g_vec2Name);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_obj) && (l_scene = Utils::Cast<ROC::IScene*>(l_obj)))
            l_scene->SetShadowsDepth(*l_val);
    }

    l_argReader.LogError();
    return 0;
}

int SceneDefs::GetShadowsQuality(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_sceneName);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_obj) && (l_scene = Utils::Cast<ROC::IScene*>(l_obj)))
            l_argReader.PushInteger(l_scene->GetShadowsQuality());
        else
        {
            l_argReader.SetError(g_sceneInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int SceneDefs::SetShadowsQuality(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    int l_val;
    l_argReader.ReadObject(l_obj, g_sceneName);
    l_argReader.ReadInteger(l_val);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_obj) && (l_scene = Utils::Cast<ROC::IScene*>(l_obj)))
            l_scene->SetShadowsQuality(l_val);
    }

    l_argReader.LogError();
    return 0;
}

int SceneDefs::AddLight(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_sceneObj;
    ROC::IElement *l_lightObj = nullptr;
    l_argReader.ReadObject(l_sceneObj, g_sceneName);
    l_argReader.ReadObject(l_lightObj, g_lightName);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_sceneObj) && (l_scene = Utils::Cast<ROC::IScene*>(l_sceneObj)))
        {
            ROC::ILight *l_light;
            if(l_lightObj && Utils::IsValid(l_lightObj) && (l_light = Utils::Cast<ROC::ILight*>(l_lightObj)))
                l_argReader.PushBoolean(l_scene->AddILight(l_light));
            else
            {
                l_argReader.SetError(g_lightInvalid);
                l_argReader.PushBoolean(false);
            }
        }
        else
        {
            l_argReader.SetError(g_sceneInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int SceneDefs::RemoveLight(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_sceneObj;
    ROC::IElement *l_lightObj = nullptr;
    l_argReader.ReadObject(l_sceneObj, g_sceneName);
    l_argReader.ReadObject(l_lightObj, g_lightName);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_sceneObj) && (l_scene = Utils::Cast<ROC::IScene*>(l_sceneObj)))
        {
            ROC::ILight *l_light;
            if(l_lightObj && Utils::IsValid(l_lightObj) && (l_light = Utils::Cast<ROC::ILight*>(l_lightObj)))
                l_argReader.PushBoolean(l_scene->RemoveILight(l_light));
            else
            {
                l_argReader.SetError(g_lightInvalid);
                l_argReader.PushBoolean(false);
            }
        }
        else
        {
            l_argReader.SetError(g_sceneInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int SceneDefs::HasLight(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_sceneObj;
    ROC::IElement *l_lightObj = nullptr;
    l_argReader.ReadObject(l_sceneObj, g_sceneName);
    l_argReader.ReadObject(l_lightObj, g_lightName);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_sceneObj) && (l_scene = Utils::Cast<ROC::IScene*>(l_sceneObj)))
        {
            ROC::ILight *l_light;
            if(l_lightObj && Utils::IsValid(l_lightObj) && (l_light = Utils::Cast<ROC::ILight*>(l_lightObj)))
                l_argReader.PushBoolean(l_scene->HasILight(l_light));
            else
            {
                l_argReader.SetError(g_lightInvalid);
                l_argReader.PushBoolean(false);
            }
        }
        else
        {
            l_argReader.SetError(g_sceneInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int SceneDefs::AddModel(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_sceneObj;
    ROC::IElement *l_modelObj = nullptr;
    l_argReader.ReadObject(l_sceneObj, g_sceneName);
    l_argReader.ReadObject(l_modelObj, g_modelName);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_sceneObj) && (l_scene = Utils::Cast<ROC::IScene*>(l_sceneObj)))
        {
            ROC::IModel *l_model;
            if(l_modelObj && Utils::IsValid(l_modelObj) && (l_model = Utils::Cast<ROC::IModel*>(l_modelObj)))
                l_argReader.PushBoolean(l_scene->AddIModel(l_model));
            else
            {
                l_argReader.SetError(g_modelInvalid);
                l_argReader.PushBoolean(false);
            }
        }
        else
        {
            l_argReader.SetError(g_sceneInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int SceneDefs::RemoveModel(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_sceneObj;
    ROC::IElement *l_modelObj = nullptr;
    l_argReader.ReadObject(l_sceneObj, g_sceneName);
    l_argReader.ReadObject(l_modelObj, g_modelName);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_sceneObj) && (l_scene = Utils::Cast<ROC::IScene*>(l_sceneObj)))
        {
            ROC::IModel *l_model;
            if(l_modelObj && Utils::IsValid(l_modelObj) && (l_model = Utils::Cast<ROC::IModel*>(l_modelObj)))
                l_argReader.PushBoolean(l_scene->RemoveIModel(l_model));
            else
            {
                l_argReader.SetError(g_modelInvalid);
                l_argReader.PushBoolean(false);
            }
        }
        else
        {
            l_argReader.SetError(g_sceneInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int SceneDefs::HasModel(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_sceneObj;
    ROC::IElement *l_modelObj = nullptr;
    l_argReader.ReadObject(l_sceneObj, g_sceneName);
    l_argReader.ReadObject(l_modelObj, g_modelName);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_sceneObj) && (l_scene = Utils::Cast<ROC::IScene*>(l_sceneObj)))
        {
            ROC::IModel *l_model;
            if(l_modelObj && Utils::IsValid(l_modelObj) && (l_model = Utils::Cast<ROC::IModel*>(l_modelObj)))
                l_argReader.PushBoolean(l_scene->HasIModel(l_model));
            else
            {
                l_argReader.SetError(g_modelInvalid);
                l_argReader.PushBoolean(false);
            }
        }
        else
        {
            l_argReader.SetError(g_sceneInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int SceneDefs::Draw(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_sceneObj;
    l_argReader.ReadObject(l_sceneObj, g_sceneName);
    if(!l_argReader.HasError())
    {
        ROC::IScene *l_scene;
        if(Utils::IsValid(l_sceneObj) && (l_scene = Utils::Cast<ROC::IScene*>(l_sceneObj)))
            l_argReader.PushBoolean(Core::GetInstance()->GetEngineCore()->GetIRenderManager()->Draw(l_scene));
        else
        {
            l_argReader.SetError(g_sceneInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int SceneDefs::IsScene(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj = nullptr;
    l_argReader.ReadNextObject(l_obj, g_sceneName);
    l_argReader.PushBoolean(l_obj && Utils::IsValid(l_obj) && Utils::Cast<ROC::IScene*>(l_obj));
    return l_argReader.GetReturnValue();
}

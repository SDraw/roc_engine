#include "stdafx.h"
#include "Lua/LuaDefs/RenderTargetDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaDefs/ElementDefs.h"
#include "Lua/LuaDefs/DrawableDefs.h"
#include "Lua/LuaArgReader.h"
#include "Core.h"
#include "Utils.h"

extern const std::string g_vec4Name;
extern const std::string g_renderTargetName("RenderTarget");
extern const std::string g_renderTargetInvalid("Invalid object/RenderTarget");

extern const std::vector<std::string> g_drawableFilterTypes;
const std::vector<std::string> g_renderTargetTypes
{
  "shadow", "rgb", "rgba", "rgbf", "rgbaf"
};
const std::vector<std::string> g_renderTargetProperties
{
    "clearColor", "depth", "color"
};

std::vector<LuaMethodDef> RenderTargetDefs::ms_metaMethods;
std::vector<LuaPropDef> RenderTargetDefs::ms_instanceProps;
std::vector<LuaMethodDef> RenderTargetDefs::ms_instanceMethods;

void RenderTargetDefs::Init()
{
    ms_instanceMethods.emplace_back("setProperty", SetProperty);

    ElementDefs::InheritTo(&ms_metaMethods, &ms_instanceProps);
    DrawableDefs::InheritTo(&ms_instanceProps, &ms_instanceMethods);
}
void RenderTargetDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_renderTargetName, Create, nullptr, nullptr, &ms_metaMethods, &ms_instanceProps, &ms_instanceMethods);
    p_vm->RegisterFunction("isRenderTarget", IsRenderTarget);
}

int RenderTargetDefs::Create(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IRenderTarget::RenderTargetType l_type;
    glm::ivec2 l_size;
    ROC::IDrawable::DrawableFilteringType l_filter = ROC::IDrawable::DFT_Linear;
    l_argReader.Skip();
    l_argReader.ReadEnum(l_type, g_renderTargetTypes);
    l_argReader.ReadInteger(l_size.x);
    l_argReader.ReadInteger(l_size.y);
    l_argReader.ReadNextEnum(l_filter, g_drawableFilterTypes);
    if(!l_argReader.HasError())
    {
        ROC::IRenderTarget *l_rt = Core::GetInstance()->GetEngineCore()->GetIElementManager()->CreateIRenderTarget(l_type, l_size, l_filter);
        if(l_rt)
            l_argReader.PushObject(Utils::Cast<ROC::IElement*>(l_rt), g_renderTargetName, true);
        else
            l_argReader.PushBoolean(false);
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int RenderTargetDefs::SetProperty(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    ROC::IRenderTarget::RenderTargetProperty l_prop;
    l_argReader.ReadObject(l_obj, g_renderTargetName);
    l_argReader.ReadEnum(l_prop, g_renderTargetProperties);
    if(!l_argReader.HasError())
    {
        ROC::IRenderTarget *l_rt;
        if(Utils::IsValid(l_obj) && (l_rt = Utils::Cast<ROC::IRenderTarget*>(l_obj)))
        {
            switch(l_prop)
            {
                case ROC::IRenderTarget::RTP_ClearColor:
                {
                    glm::vec4 *l_vec;
                    l_argReader.ReadObject(l_vec, g_vec4Name);
                    if(!l_argReader.HasError())
                        l_argReader.PushBoolean(l_rt->SetProperty(l_prop, l_vec));
                    else
                        l_argReader.PushBoolean(false);
                } break;

                case ROC::IRenderTarget::RTP_Depth:
                case ROC::IRenderTarget::RTP_Color:
                {
                    bool l_state;
                    l_argReader.ReadBoolean(l_state);
                    if(!l_argReader.HasError())
                        l_argReader.PushBoolean(l_rt->SetProperty(l_prop, &l_state));
                    else
                        l_argReader.PushBoolean(false);
                } break;

                default:
                    l_argReader.PushBoolean(false);
                    break;
            }
        }
        else
        {
            l_argReader.SetError(g_renderTargetInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int RenderTargetDefs::IsRenderTarget(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj = nullptr;
    l_argReader.ReadNextObject(l_obj, g_renderTargetName);
    l_argReader.PushBoolean(l_obj && Utils::IsValid(l_obj) && Utils::Cast<ROC::IRenderTarget*>(l_obj));
    return l_argReader.GetReturnValue();
}

#include "stdafx.h"
#include "Lua/LuaDefs/DrawableDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaArgReader.h"
#include "Core.h"
#include "Utils.h"

extern const std::string g_vec2Name;
extern const std::string g_vec3Name;
extern const std::string g_vec4Name;
extern const std::string g_quatName;
extern const std::string g_drawableName("Drawable");
extern const std::string g_drawableInvalid("Invalid object/Drawable");

extern const std::vector<std::string> g_drawableFilterTypes
{
    "nearest", "linear"
};

std::vector<LuaPropDef> DrawableDefs::ms_instanceProps;
std::vector<LuaMethodDef> DrawableDefs::ms_instanceMethods;

void DrawableDefs::Init()
{
    ms_instanceProps.emplace_back("isTransparent", GetIsTransparent, nullptr);
    ms_instanceProps.emplace_back("isCubic", GetIsCubic, nullptr);
    ms_instanceProps.emplace_back("size", GetSize, nullptr);
    ms_instanceProps.emplace_back("filtering", GetFiltering, nullptr);

    ms_instanceMethods.emplace_back("draw", Draw);
}
void DrawableDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_drawableName, nullptr, nullptr, nullptr, nullptr, &ms_instanceProps, nullptr);
}
void DrawableDefs::InheritTo(std::vector<LuaPropDef> *p_instanceProps, std::vector<LuaMethodDef> *p_instanceMethods)
{
    if(p_instanceProps)
        p_instanceProps->insert(p_instanceProps->end(), ms_instanceProps.begin(), ms_instanceProps.end());
    if(p_instanceMethods)
        p_instanceMethods->insert(p_instanceMethods->end(), ms_instanceMethods.begin(), ms_instanceMethods.end());
}

int DrawableDefs::GetIsTransparent(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj);
    if(!l_argReader.HasError())
    {
        ROC::IDrawable *l_drawable;
        if(Utils::IsValid(l_obj) && (l_drawable = Utils::Cast<ROC::IDrawable*>(l_obj)))
            l_argReader.PushBoolean(l_drawable->IsTransparent());
        else
        {
            l_argReader.SetError(g_drawableInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int DrawableDefs::GetIsCubic(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj);
    if(!l_argReader.HasError())
    {
        ROC::IDrawable *l_drawable;
        if(Utils::IsValid(l_obj) && (l_drawable = Utils::Cast<ROC::IDrawable*>(l_obj)))
            l_argReader.PushBoolean(l_drawable->IsCubic());
        else
        {
            l_argReader.SetError(g_drawableInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int DrawableDefs::GetSize(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj);
    if(!l_argReader.HasError())
    {
        ROC::IDrawable *l_drawable;
        if(Utils::IsValid(l_obj) && (l_drawable = Utils::Cast<ROC::IDrawable*>(l_obj)))
            l_argReader.PushObject(new glm::vec2(l_drawable->GetSize()), g_vec2Name, false);
        else
        {
            l_argReader.SetError(g_drawableInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int DrawableDefs::GetFiltering(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj);
    if(!l_argReader.HasError())
    {
        ROC::IDrawable *l_drawable;
        if(Utils::IsValid(l_obj) && (l_drawable = Utils::Cast<ROC::IDrawable*>(l_obj)))
            l_argReader.PushString(g_drawableFilterTypes[l_drawable->GetFiltering()]);
        else
        {
            l_argReader.SetError(g_drawableInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int DrawableDefs::Draw(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec2 *l_pos;
    glm::vec2 *l_size;
    float l_rot = 0.f;
    glm::vec4 *l_col;
    l_argReader.ReadObject(l_obj);
    l_argReader.ReadObject(l_pos, g_vec2Name);
    l_argReader.ReadObject(l_size, g_vec2Name);
    l_argReader.ReadNextNumber(l_rot);
    l_argReader.ReadNextObject(l_col, g_vec4Name);
    if(!l_argReader.HasError())
    {
        ROC::IDrawable *l_drawable;
        if(Utils::IsValid(l_obj) && (l_drawable = Utils::Cast<ROC::IDrawable*>(l_obj)))
            l_argReader.PushBoolean(Core::GetInstance()->GetEngineCore()->GetIRenderManager()->Draw(l_drawable, *l_pos, *l_size, l_rot, l_col ? *l_col : glm::vec4(1.f)));
        else
        {
            l_argReader.SetError(g_drawableInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int DrawableDefs::IsDrawable(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj = nullptr;
    l_argReader.ReadNextObject(l_obj);
    l_argReader.PushBoolean(l_obj && Utils::IsValid(l_obj) && Utils::Cast<ROC::IDrawable*>(l_obj));
    return l_argReader.GetReturnValue();
}

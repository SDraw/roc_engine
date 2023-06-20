#include "stdafx.h"
#include "Lua/LuaDefs/FontDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaDefs/ElementDefs.h"
#include "Lua/LuaArgReader.h"
#include "Core.h"
#include "Utils.h"

extern const std::string g_vec2Name;
extern const std::string g_vec4Name;
const std::string g_fontName("Font");
const std::string g_fontInvalid("Invalid object/Font");

const std::vector<std::string> g_fontFilteringTypes
{
    "nearest", "linear"
};

std::vector<LuaMethodDef> FontDefs::ms_metaMethods;
std::vector<LuaPropDef> FontDefs::ms_instanceProps;
std::vector<LuaMethodDef> FontDefs::ms_instanceMethods;


void FontDefs::Init()
{
    ms_instanceProps.emplace_back("filtering", GetFiltering, nullptr);
    ms_instanceProps.emplace_back("glyphSize", GetGlyphSize, nullptr);

    ms_instanceMethods.emplace_back("draw", Draw);

    ElementDefs::InheritTo(&ms_metaMethods, &ms_instanceProps);
}
void FontDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_fontName, Create, nullptr, nullptr, &ms_metaMethods, &ms_instanceProps, &ms_instanceMethods);
    p_vm->RegisterFunction("isFont", IsFont);
}

int FontDefs::Create(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    std::string l_path;
    int l_glyph;
    glm::ivec2 l_atlas(256, 256);
    ROC::IFont::FontFilteringType l_filter = ROC::IFont::FFT_Nearest;
    l_argReader.Skip();
    l_argReader.ReadString(l_path);
    l_argReader.ReadInteger(l_glyph);
    l_argReader.ReadNextInteger(l_atlas.x);
    l_argReader.ReadNextInteger(l_atlas.y);
    l_argReader.ReadNextEnum(l_filter, g_fontFilteringTypes);
    if(!l_argReader.HasError())
    {
        ROC::IFont *l_font = Core::GetInstance()->GetEngineCore()->GetIElementManager()->CreateIFont(l_path.c_str(), l_glyph, l_atlas, l_filter);
        if(l_font)
            l_argReader.PushObject(Utils::Cast<ROC::IElement*>(l_font), g_fontName, true);
        else
            l_argReader.PushBoolean(false);
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int FontDefs::GetFiltering(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_fontName);
    if(!l_argReader.HasError())
    {
        ROC::IFont *l_font;
        if(Utils::IsValid(l_obj) && (l_font = Utils::Cast<ROC::IFont*>(l_obj)))
            l_argReader.PushString(g_fontFilteringTypes[l_font->GetFiltering()]);
        else
        {
            l_argReader.SetError(g_fontInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int FontDefs::GetGlyphSize(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_fontName);
    if(!l_argReader.HasError())
    {
        ROC::IFont *l_font;
        if(Utils::IsValid(l_obj) && (l_font = Utils::Cast<ROC::IFont*>(l_obj)))
            l_argReader.PushNumber(l_font->GetGlyphSize());
        else
        {
            l_argReader.SetError(g_fontInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int FontDefs::Draw(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    std::string l_text;
    glm::vec2 *l_pos;
    glm::vec4 *l_col = nullptr;
    l_argReader.ReadObject(l_obj, g_fontName);
    l_argReader.ReadString(l_text);
    l_argReader.ReadObject(l_pos, g_vec2Name);
    l_argReader.ReadNextObject(l_col, g_vec4Name);
    if(!l_argReader.HasError())
    {
        ROC::IFont *l_font;
        if(Utils::IsValid(l_obj) && (l_font = Utils::Cast<ROC::IFont*>(l_obj)))
            l_argReader.PushBoolean(Core::GetInstance()->GetEngineCore()->GetIRenderManager()->Draw(l_font, *l_pos, l_text.c_str(), l_col ? *l_col : glm::vec4(1.f)));
        else
        {
            l_argReader.SetError(g_fontInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int FontDefs::IsFont(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj = nullptr;
    l_argReader.ReadNextObject(l_obj, g_fontName);
    l_argReader.PushBoolean(l_obj && Utils::IsValid(l_obj) && Utils::Cast<ROC::IFont*>(l_obj));
    return l_argReader.GetReturnValue();
}
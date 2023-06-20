#include "stdafx.h"
#include "Lua/LuaDefs/TextureDefs.h"
#include "Lua/LuaDefs/ElementDefs.h"
#include "Lua/LuaDefs/DrawableDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaArgReader.h"
#include "Core.h"
#include "Utils.h"

extern const std::string g_textureName("Texture");
extern const std::string g_textureInvalid("Invalid object/Texture");

extern const std::vector<std::string> g_drawableFilterTypes;
const std::vector<std::string> g_textureTypes
{
    "rgb", "rgba", "cubemap"
};

std::vector<LuaMethodDef> TextureDefs::ms_metaMethods;
std::vector<LuaPropDef> TextureDefs::ms_instanceProps;
std::vector<LuaMethodDef> TextureDefs::ms_instanceMethods;

void TextureDefs::Init()
{
    ms_instanceProps.emplace_back("isCompressed", GetIsCompressed, nullptr);

    ElementDefs::InheritTo(&ms_metaMethods, &ms_instanceProps);
    DrawableDefs::InheritTo(&ms_instanceProps, &ms_instanceMethods);
}
void TextureDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_textureName, Create, nullptr, nullptr, &ms_metaMethods, &ms_instanceProps, nullptr);
    p_vm->RegisterFunction("isTexture", IsTexture);
}

int TextureDefs::Create(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    std::string l_path;
    ROC::ITexture::TextureType l_type;
    ROC::IDrawable::DrawableFilteringType l_filter = ROC::IDrawable::DFT_Linear;
    bool l_compress = false;
    l_argReader.Skip();
    l_argReader.ReadString(l_path);
    l_argReader.ReadEnum(l_type, g_textureTypes);
    l_argReader.ReadNextEnum(l_filter, g_drawableFilterTypes);
    l_argReader.ReadNextBoolean(l_compress);
    if(!l_argReader.HasError())
    {
        switch(l_type)
        {
            case ROC::ITexture::TT_RGB:
            case ROC::ITexture::TT_RGBA:
            {
                ROC::ITexture* l_tex = Core::GetInstance()->GetEngineCore()->GetIElementManager()->CreateITexture(l_path.c_str(), l_type, l_filter, l_compress);
                if(l_tex)
                    l_argReader.PushObject(Utils::Cast<ROC::IElement*>(l_tex), g_textureName, true);
                else
                    l_argReader.PushBoolean(false);
            } break;

            default:
                l_argReader.PushBoolean(false);
                break;
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
   
}

int TextureDefs::GetIsCompressed(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_textureName);
    if(!l_argReader.HasError())
    {
        ROC::ITexture* l_tex;
        if(Utils::IsValid(l_obj) && (l_tex = Utils::Cast<ROC::ITexture*>(l_obj)))
            l_argReader.PushBoolean(l_tex->IsCompressed());
        else
            l_argReader.PushBoolean(false);
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int TextureDefs::IsTexture(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj = nullptr;
    l_argReader.ReadNextObject(l_obj);
    l_argReader.PushBoolean(l_obj && Utils::IsValid(l_obj) && Utils::Cast<ROC::ITexture*>(l_obj));
    return l_argReader.GetReturnValue();
}

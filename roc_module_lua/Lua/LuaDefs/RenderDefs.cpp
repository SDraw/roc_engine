#include "stdafx.h"
#include "Lua/LuaDefs/RenderDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaArgReader.h"
#include "Core.h"
#include "Utils.h"

extern const std::string g_vec4Name;
const std::string g_renderName("Render");

const std::vector<std::string> g_polygonModes
{
    "point", "line", "fill"
};

std::vector<LuaPropDef> RenderDefs::ms_staticProps;
std::vector<LuaMethodDef> RenderDefs::ms_staticMethods;

void RenderDefs::Init()
{
    ms_staticProps.emplace_back("polygonMode", nullptr, SetPolygonMode);
    ms_staticProps.emplace_back("clearColor", nullptr, SetClearColor);
    ms_staticProps.emplace_back("viewport", nullptr, SetViewport);

    ms_staticMethods.emplace_back("clearViewport", ClearViewport);
}
void RenderDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_renderName, nullptr, &ms_staticProps, &ms_staticMethods, nullptr, nullptr, nullptr);
}

int RenderDefs::SetPolygonMode(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    int l_mode;
    l_argReader.ReadEnum(l_mode, g_polygonModes);
    if(!l_argReader.HasError())
        Core::GetInstance()->GetEngineCore()->GetIRenderManager()->SetPolygonMode(l_mode);

    l_argReader.LogError();
    return 0;
}

int RenderDefs::SetClearColor(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec4 *l_vec;
    l_argReader.ReadObject(l_vec, g_vec4Name);
    if(!l_argReader.HasError())
        Core::GetInstance()->GetEngineCore()->GetIRenderManager()->SetClearColor(*l_vec);

    l_argReader.LogError();
    return 0;
}

int RenderDefs::SetViewport(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec4 *l_vec;
    l_argReader.ReadObject(l_vec, g_vec4Name);
    if(!l_argReader.HasError())
    {
        glm::ivec4 l_ivec(*l_vec);
        Core::GetInstance()->GetEngineCore()->GetIRenderManager()->SetViewport(l_ivec);
    }

    l_argReader.LogError();
    return 0;
}

int RenderDefs::ClearViewport(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    bool l_depth = true;
    bool l_color = true;
    l_argReader.ReadNextBoolean(l_depth);
    l_argReader.ReadNextBoolean(l_color);
    l_argReader.PushBoolean(Core::GetInstance()->GetEngineCore()->GetIRenderManager()->ClearViewport(l_depth, l_color));
    return l_argReader.GetReturnValue();
}

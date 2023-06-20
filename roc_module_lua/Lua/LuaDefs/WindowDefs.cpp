#include "stdafx.h"
#include "Lua/LuaDefs/WindowDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaArgReader.h"
#include "Core.h"
#include "Utils.h"

extern const std::string g_vec2Name;
const std::string g_windowName("Window");

std::vector<LuaPropDef> WindowDefs::ms_staticProps;
std::vector<LuaMethodDef> WindowDefs::ms_staticMethods;

void WindowDefs::Init()
{
    ms_staticProps.emplace_back("position", GetPosition, SetPosition);
    ms_staticProps.emplace_back("size", GetSize, nullptr);
    ms_staticProps.emplace_back("vsync", nullptr, SetVSync);
    ms_staticProps.emplace_back("frameLimit", GetFrameLimit, SetFrameLimit);
    ms_staticProps.emplace_back("title", nullptr, SetTitle);
    ms_staticProps.emplace_back("icon", nullptr, SetIcon);
    ms_staticProps.emplace_back("focus", GetFocus, nullptr);
    ms_staticProps.emplace_back("cursorPosition", GetCursorPosition, SetCursorPosition);

    ms_staticMethods.emplace_back("close", Close);
    ms_staticMethods.emplace_back("requestFocus", RequestFocus);
    ms_staticMethods.emplace_back("setCursorMode", SetCursorMode);
}
void WindowDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_windowName, nullptr, &ms_staticProps, &ms_staticMethods, nullptr, nullptr, nullptr);
}

int WindowDefs::GetPosition(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::ivec2 l_pos;
    Core::GetInstance()->GetEngineCore()->GetISfmlManager()->GetWindowPosition(l_pos);
    l_argReader.PushObject(new glm::vec2(l_pos), g_vec2Name, false);
    return 1;
}
int WindowDefs::SetPosition(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec2 *l_pos;
    l_argReader.ReadObject(l_pos, g_vec2Name);
    if(!l_argReader.HasError())
    {
        glm::ivec2 l_ipos(*l_pos);
        Core::GetInstance()->GetEngineCore()->GetISfmlManager()->SetWindowPosition(l_ipos);
    }

    l_argReader.LogError();
    return 0;
}

int WindowDefs::GetSize(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::ivec2 l_vec;
    Core::GetInstance()->GetEngineCore()->GetISfmlManager()->GetWindowSize(l_vec);
    l_argReader.PushObject(new glm::vec2(l_vec), g_vec2Name, false);
    return 1;
}

int WindowDefs::SetVSync(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    bool l_state;
    l_argReader.ReadBoolean(l_state);
    if(!l_argReader.HasError())
        Core::GetInstance()->GetEngineCore()->GetISfmlManager()->SetVSync(l_state);

    l_argReader.LogError();
    return 0;
}

int WindowDefs::GetFrameLimit(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushInteger(static_cast<int>(Core::GetInstance()->GetEngineCore()->GetISfmlManager()->GetFramelimit()));
    return 1;
}
int WindowDefs::SetFrameLimit(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    int l_val;
    l_argReader.ReadInteger(l_val);
    if(!l_argReader.HasError())
        Core::GetInstance()->GetEngineCore()->GetISfmlManager()->SetFramelimit(static_cast<unsigned int>(l_val));

    l_argReader.LogError();
    return 0;
}

int WindowDefs::SetTitle(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    std::string l_val;
    l_argReader.ReadString(l_val);
    if(!l_argReader.HasError())
        Core::GetInstance()->GetEngineCore()->GetISfmlManager()->SetTitle(l_val.c_str());

    l_argReader.LogError();
    return 0;
}

int WindowDefs::SetIcon(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    std::string l_val;
    l_argReader.ReadString(l_val);
    if(!l_argReader.HasError())
        Core::GetInstance()->GetEngineCore()->GetISfmlManager()->SetIcon(l_val.c_str());

    l_argReader.LogError();
    return 0;
}

int WindowDefs::GetFocus(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushBoolean(Core::GetInstance()->GetEngineCore()->GetISfmlManager()->GetFocusState());
    return 1;
}

int WindowDefs::GetCursorPosition(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::ivec2 l_pos;
    Core::GetInstance()->GetEngineCore()->GetISfmlManager()->GetCursorPosition(l_pos);
    l_argReader.PushObject(new glm::vec2(l_pos), g_vec2Name, false);
    return 1;
}
int WindowDefs::SetCursorPosition(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec2 *l_pos;
    l_argReader.ReadObject(l_pos, g_vec2Name);
    if(!l_argReader.HasError())
    {
        glm::ivec2 l_ipos(*l_pos);
        Core::GetInstance()->GetEngineCore()->GetISfmlManager()->SetCursorPosition(l_ipos);
    }

    l_argReader.LogError();
    return 0;
}

int WindowDefs::Close(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    Core::GetInstance()->GetEngineCore()->GetISfmlManager()->CloseWindow();
    l_argReader.PushBoolean(true);
    return l_argReader.GetReturnValue();
}

int WindowDefs::RequestFocus(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    Core::GetInstance()->GetEngineCore()->GetISfmlManager()->RequestFocus();
    l_argReader.PushBoolean(true);
    return l_argReader.GetReturnValue();
}

int WindowDefs::SetCursorMode(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    bool l_stateA;
    bool l_stateB;
    l_argReader.ReadBoolean(l_stateA);
    l_argReader.ReadBoolean(l_stateB);
    if(!l_argReader.HasError())
    {
        Core::GetInstance()->GetEngineCore()->GetISfmlManager()->SetCursorMode(l_stateA, l_stateB);
        l_argReader.PushBoolean(true);
    }
    else
        l_argReader.PushBoolean(false);

    return l_argReader.GetReturnValue();
}

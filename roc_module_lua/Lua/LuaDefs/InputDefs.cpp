#include "stdafx.h"
#include "Lua/LuaDefs/InputDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaArgReader.h"
#include "Core.h"
#include "Utils.h"

const std::string g_inputName("Input");

const std::vector<std::string> g_inputKeys =
{
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
    "esc", "lctrl", "lshift", "lalt", "lsys", "rctrl", "rshift", "ralt", "rsys", "menu",
    "lbracket", "rbracket", "semicolon", "comma", "period", "quote", "slash", "backslash", "tilde", "equal", "dash",
    "space", "return", "backspace", "tab", "pgup", "pgdn", "end", "home", "insert", "delete",
    "num+", "num-", "num*", "num/",
    "arrow_l", "arrow_r", "arrow_u", "arrow_d",
    "num0", "num1", "num2", "num3", "num4", "num5", "num6", "num7", "num8", "num9",
    "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15",
    "pause"
};
const std::vector<std::string> g_inputMouseKeys =
{
    "left", "right", "middle",
    "x1", "x2"
};
const std::vector<std::string> g_inputJoypadAxes =
{
    "x", "y", "z",
    "r", "u", "v",
    "povx", "povy"
};

std::vector<LuaPropDef> InputDefs::ms_staticProps;
std::vector<LuaMethodDef> InputDefs::ms_staticMethods;

void InputDefs::Init()
{
    ms_staticProps.emplace_back("enabled", GetEnabled, SetEnabled);
    ms_staticProps.emplace_back("clipboard", GetClipboard, SetClipboard);

    ms_staticMethods.emplace_back("getKey", GetKey);
    ms_staticMethods.emplace_back("getMouseKey", GetMouseKey);
    ms_staticMethods.emplace_back("isJoypadConnected", IsJoypadConnected);
    ms_staticMethods.emplace_back("getJoypadKey", GetJoypadKey);
    ms_staticMethods.emplace_back("getJoypadButtonCount", GetJoypadKey);
    ms_staticMethods.emplace_back("getJoypadAxis", GetJoypadAxis);
}
void InputDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_inputName, nullptr, &ms_staticProps, &ms_staticMethods, nullptr, nullptr, nullptr);
}

int InputDefs::GetEnabled(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushBoolean(Core::GetInstance()->GetEngineCore()->GetISfmlManager()->GetInputEnabled());
    return 1;
}
int InputDefs::SetEnabled(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    bool l_state;
    l_argReader.ReadBoolean(l_state);
    if(!l_argReader.HasError())
        Core::GetInstance()->GetEngineCore()->GetISfmlManager()->SetInputEnabled(l_state);

    l_argReader.LogError();
    return 0;
}

int InputDefs::GetClipboard(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushString(Core::GetInstance()->GetEngineCore()->GetISfmlManager()->GetClipboardString());
    return 1;
}
int InputDefs::SetClipboard(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    std::string l_str;
    l_argReader.ReadString(l_str);
    if(!l_argReader.HasError())
        Core::GetInstance()->GetEngineCore()->GetISfmlManager()->SetClipboardString(l_str.c_str());

    l_argReader.LogError();
    return 0;
}

int InputDefs::GetKey(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    int l_key;
    if(l_argReader.IsNextString())
        l_argReader.ReadEnum(l_key, g_inputKeys);
    else
        l_argReader.ReadInteger(l_key);
    if(!l_argReader.HasError())
        l_argReader.PushBoolean(Core::GetInstance()->GetEngineCore()->GetISfmlManager()->IsKeyPressed(l_key));
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int InputDefs::GetMouseKey(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    int l_key;
    if(l_argReader.IsNextString())
        l_argReader.ReadEnum(l_key, g_inputMouseKeys);
    else
        l_argReader.ReadInteger(l_key);
    if(!l_argReader.HasError())
        l_argReader.PushBoolean(Core::GetInstance()->GetEngineCore()->GetISfmlManager()->IsMouseKeyPressed(l_key));
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int InputDefs::IsJoypadConnected(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    int l_key;
    l_argReader.ReadInteger(l_key);
    if(!l_argReader.HasError())
        l_argReader.PushBoolean(Core::GetInstance()->GetEngineCore()->GetISfmlManager()->IsMouseKeyPressed(l_key));
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int InputDefs::GetJoypadKey(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    int l_jp;
    int l_key;
    l_argReader.ReadInteger(l_jp);
    l_argReader.ReadInteger(l_key);
    if(!l_argReader.HasError())
        l_argReader.PushBoolean(Core::GetInstance()->GetEngineCore()->GetISfmlManager()->GetJoypadButtonState(l_jp,l_key));
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int InputDefs::GetJoypadButtonCount(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    int l_jp;
    l_argReader.ReadInteger(l_jp);
    if(!l_argReader.HasError())
        l_argReader.PushInteger(static_cast<int>(Core::GetInstance()->GetEngineCore()->GetISfmlManager()->GetJoypadButtonCount(l_jp)));
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int InputDefs::IsJoypadAxisPresent(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    int l_jp;
    int l_axis;
    l_argReader.ReadInteger(l_jp);
    if(l_argReader.IsNextString())
        l_argReader.ReadEnum(l_axis, g_inputJoypadAxes);
    else
        l_argReader.ReadInteger(l_axis);
    if(!l_argReader.HasError())
        l_argReader.PushBoolean(Core::GetInstance()->GetEngineCore()->GetISfmlManager()->CheckJoypadAxis(l_jp,l_axis));
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}
int InputDefs::GetJoypadAxis(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    int l_jp;
    int l_axis;
    l_argReader.ReadInteger(l_jp);
    if(l_argReader.IsNextString())
        l_argReader.ReadEnum(l_axis, g_inputJoypadAxes);
    else
        l_argReader.ReadInteger(l_axis);
    if(!l_argReader.HasError())
        l_argReader.PushNumber(Core::GetInstance()->GetEngineCore()->GetISfmlManager()->GetJoypadAxisValue(l_jp, l_axis));
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}


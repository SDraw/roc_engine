#include "stdafx.h"
#include "Lua/LuaDefs/TimeDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaArgReader.h"
#include "Core.h"
#include "Utils.h"

const std::string g_timeName("Time");

std::vector<LuaPropDef> TimeDefs::ms_staticProps;
std::vector<LuaMethodDef> TimeDefs::ms_staticMethods;

void TimeDefs::Init()
{
    ms_staticProps.emplace_back("deltaTime", GetDeltaTime, nullptr);
    ms_staticProps.emplace_back("totalTime", GetTotalTime, nullptr);

    ms_staticMethods.emplace_back("getSystemTime", GetSystemTime);
}
void TimeDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_timeName, nullptr, &ms_staticProps, &ms_staticMethods, nullptr, nullptr, nullptr);
}

int TimeDefs::GetDeltaTime(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushNumber(Core::GetInstance()->GetEngineCore()->GetEngineDelta());
    return 1;
}

int TimeDefs::GetTotalTime(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushNumber(Core::GetInstance()->GetEngineCore()->GetISfmlManager()->GetTime());
    return 1;
}

int TimeDefs::GetSystemTime(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    std::time_t l_timeRaw = std::time(nullptr);
    tm l_time = { 0 };
    localtime_s(&l_time, &l_timeRaw);
    l_argReader.PushInteger(l_time.tm_hour);
    l_argReader.PushInteger(l_time.tm_min);
    l_argReader.PushInteger(l_time.tm_sec);
    return l_argReader.GetReturnValue();
}

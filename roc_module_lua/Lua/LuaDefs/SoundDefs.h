#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class SoundDefs final
{
    static std::vector<LuaPropDef> ms_staticProps;
    static std::vector<LuaMethodDef> ms_metaMethods;
    static std::vector<LuaPropDef> ms_instanceProps;
    static std::vector<LuaMethodDef> ms_instanceMethods;

    static int Create(lua_State *p_state);
    static int GetListenerPosition(lua_State *p_state);
    static int SetListenerPosition(lua_State *p_state);
    static int GetListenerDirection(lua_State *p_state);
    static int SetListenerDirection(lua_State *p_state);
    static int GetListenerUp(lua_State *p_state);
    static int SetListenerUp(lua_State *p_state);
    static int GetGlobalVolume(lua_State *p_state);
    static int SetGlobalVolume(lua_State *p_state);
    static int GetLoop(lua_State *p_state);
    static int SetLoop(lua_State *p_state);
    static int GetDuration(lua_State *p_state);
    static int GetSpeed(lua_State *p_state);
    static int SetSpeed(lua_State *p_state);
    static int GetVolume(lua_State *p_state);
    static int SetVolume(lua_State *p_state);
    static int GetTime(lua_State *p_state);
    static int SetTime(lua_State *p_state);
    static int Get3D(lua_State *p_state);
    static int Set3D(lua_State *p_state);
    static int GetPosition(lua_State *p_state);
    static int SetPosition(lua_State *p_state);
    static int GetDistance(lua_State *p_state);
    static int SetDistance(lua_State *p_state);
    static int GetState(lua_State *p_state);
    static int Play(lua_State *p_state);
    static int Pause(lua_State *p_state);
    static int Stop(lua_State *p_state);
    static int IsSound(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};

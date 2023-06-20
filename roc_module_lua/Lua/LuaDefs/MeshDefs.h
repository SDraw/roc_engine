#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class MeshDefs final
{
    static std::vector<LuaMethodDef> ms_metaMethods;
    static std::vector<LuaPropDef> ms_instanceProps;
    
    MeshDefs() = delete;
    ~MeshDefs() = delete;

    static int Create(lua_State *p_state);
    static int GetBoundSphereRadius(lua_State *p_state);
    static int GetMaterialsCount(lua_State *p_state);
    static int IsMesh(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};

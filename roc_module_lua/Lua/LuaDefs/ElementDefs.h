#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class ElementDefs final
{
    static std::vector<LuaMethodDef> ms_staticMethods;
    static std::vector<LuaMethodDef> ms_metaMethods;
    static std::vector<LuaPropDef> ms_instanceProps;

    ElementDefs() = delete;
    ~ElementDefs() = delete;

    static int Destroy(lua_State *p_state);
    static int Equals(lua_State *p_state);
    static int GetType(lua_State *p_state);
    static int IsElement(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
    static void InheritTo(std::vector<LuaMethodDef> *p_meta, std::vector<LuaPropDef> *p_instanceProps);
};


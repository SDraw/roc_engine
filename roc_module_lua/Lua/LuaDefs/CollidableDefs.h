#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class CollidableDefs final
{
    static std::vector<LuaMethodDef> ms_instanceMethods;

    CollidableDefs() = delete;
    ~CollidableDefs() = delete;

    static int SetCollidableWith(lua_State *p_state);
    static int IsCollidable(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
    static void InheritTo(std::vector<LuaMethodDef> *p_instanceMethods);
};


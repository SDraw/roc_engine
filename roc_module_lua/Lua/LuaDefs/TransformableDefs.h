#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class TransformableDefs final
{
    static std::vector<LuaPropDef> ms_instanceProps;

    TransformableDefs() = delete;
    ~TransformableDefs() = delete;

    static int GetPosition(lua_State *p_state);
    static int SetPosition(lua_State *p_state);
    static int GetRotation(lua_State *p_state);
    static int SetRotation(lua_State *p_state);
    static int GetScale(lua_State *p_state);
    static int SetScale(lua_State *p_state);
    static int GetMatrix(lua_State *p_state);
    static int IsTransformable(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
    static void InheritTo(std::vector<LuaPropDef> *p_instanceProps);
};


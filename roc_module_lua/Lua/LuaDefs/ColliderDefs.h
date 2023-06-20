#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM;

class ColliderDefs final
{
    static std::vector<LuaMethodDef> ms_metaMethods;
    static std::vector<LuaPropDef> ms_instanceProps;
    static std::vector<LuaMethodDef> ms_instanceMethods;

    ColliderDefs() = delete;
    ~ColliderDefs() = delete;

    static int Create(lua_State *p_state);
    static int GetVelocity(lua_State *p_state);
    static int SetVelocity(lua_State *p_state);
    static int GetAngularVelocity(lua_State *p_state);
    static int SetAngularVelocity(lua_State *p_state);
    static int GetLinearFactor(lua_State *p_state);
    static int SetLinearFactor(lua_State *p_state);
    static int GetMass(lua_State *p_state);
    static int GetFriction(lua_State *p_state);
    static int SetFriction(lua_State *p_state);
    static int GetRestitution(lua_State *p_state);
    static int SetRestitution(lua_State *p_state);
    static int GetMotionType(lua_State *p_state);
    static int SetMotionType(lua_State *p_state);
    static int ApplyForce(lua_State *p_state);
    static int ApplyCentralForce(lua_State *p_state);
    static int ApplyImpulse(lua_State *p_state);
    static int ApplyCentralImpulse(lua_State *p_state);
    static int ApplyTorque(lua_State *p_state);
    static int IsCollider(lua_State *p_state);
public:
    static void Init();
    static void RegisterInVM(LuaVM *p_vm);
};


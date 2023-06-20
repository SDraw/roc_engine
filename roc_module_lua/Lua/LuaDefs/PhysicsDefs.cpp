#include "stdafx.h"
#include "Lua/LuaDefs/PhysicsDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaArgReader.h"
#include "Core.h"
#include "Utils.h"

extern const std::string g_vec3Name;
const std::string g_physicsName("Physics");

std::vector<LuaPropDef> PhysicsDefs::ms_staticProps;
std::vector<LuaMethodDef> PhysicsDefs::ms_staticMethods;

void PhysicsDefs::Init()
{
    ms_staticProps.emplace_back("enabled", GetEnabled, SetEnabled);
    ms_staticProps.emplace_back("floorEnabled", GetFloorEnabled, SetFloorEnabled);
    ms_staticProps.emplace_back("gravity", GetGravity, SetGravity);

    ms_staticMethods.emplace_back("raycast", Raycast);
}

void PhysicsDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_physicsName, nullptr, &ms_staticProps, &ms_staticMethods, nullptr, nullptr, nullptr);
}

int PhysicsDefs::GetEnabled(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushBoolean(Core::GetInstance()->GetEngineCore()->GetIPhysicsManager()->GetPhysicsEnabled());
    return 1;
}
int PhysicsDefs::SetEnabled(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    bool l_state;
    l_argReader.ReadBoolean(l_state);
    if(!l_argReader.HasError())
        Core::GetInstance()->GetEngineCore()->GetIPhysicsManager()->SetPhysicsEnabled(l_state);

    l_argReader.LogError();
    return 0;
}

int PhysicsDefs::GetFloorEnabled(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    l_argReader.PushBoolean(Core::GetInstance()->GetEngineCore()->GetIPhysicsManager()->GetFloorEnabled());
    return 1;
}
int PhysicsDefs::SetFloorEnabled(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    bool l_state;
    l_argReader.ReadBoolean(l_state);
    if(!l_argReader.HasError())
        Core::GetInstance()->GetEngineCore()->GetIPhysicsManager()->SetFloorEnabled(l_state);

    l_argReader.LogError();
    return 0;
}

int PhysicsDefs::GetGravity(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 l_grav;
    Core::GetInstance()->GetEngineCore()->GetIPhysicsManager()->GetGravity(l_grav);
    l_argReader.PushObject(new glm::vec3(l_grav), g_vec3Name, false);
    return 1;
}
int PhysicsDefs::SetGravity(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_grav;
    l_argReader.ReadObject(l_grav, g_vec3Name);
    if(!l_argReader.HasError())
        Core::GetInstance()->GetEngineCore()->GetIPhysicsManager()->SetGravity(*l_grav);

    l_argReader.LogError();
    return 0;
}

int PhysicsDefs::Raycast(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    glm::vec3 *l_start;
    glm::vec3 *l_end;
    l_argReader.ReadObject(l_start, g_vec3Name);
    l_argReader.ReadObject(l_end, g_vec3Name);
    if(!l_argReader.HasError())
    {
        glm::vec3 l_normal;
        glm::vec3 l_hitEnd(*l_end);
        ROC::IElement *l_what;
        if(Core::GetInstance()->GetEngineCore()->GetIPhysicsManager()->RayCast(*l_start, l_hitEnd, l_normal, l_what))
        {
            l_argReader.PushBoolean(true);
            l_argReader.PushObject(new glm::vec3(l_hitEnd), g_vec3Name, false);
            l_argReader.PushObject(new glm::vec3(l_normal), g_vec3Name, false);
            if(l_what)
                l_argReader.PushObject(l_what, l_what->GetElementTypeName(), true);
            else
                l_argReader.PushBoolean(false);
        }
        else
            l_argReader.PushBoolean(false);
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

#include "stdafx.h"
#include "Lua/LuaDefs/ColliderDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaDefs/ElementDefs.h"
#include "Lua/LuaDefs/TransformableDefs.h"
#include "Lua/LuaDefs/CollidableDefs.h"
#include "Lua/LuaArgReader.h"
#include "Core.h"
#include "Utils.h"

extern const std::string g_vec3Name;
const std::string g_colliderName("Collider");
const std::string g_colliderInvalid("Invalid object/Collider");

const std::vector<std::string> g_colliderTypes
{
    "sphere", "box", "cylinder", "capsule", "cone"
};
const std::vector<std::string> g_colliderMotionTypes
{
    "default", "static", "kinematic"
};

std::vector<LuaMethodDef> ColliderDefs::ms_metaMethods;
std::vector<LuaPropDef> ColliderDefs::ms_instanceProps;
std::vector<LuaMethodDef> ColliderDefs::ms_instanceMethods;

void ColliderDefs::Init()
{
    ms_instanceProps.emplace_back("velocity", GetVelocity, SetVelocity);
    ms_instanceProps.emplace_back("angularVelocity", GetAngularVelocity, SetAngularVelocity);
    ms_instanceProps.emplace_back("linearFactor", GetLinearFactor, SetLinearFactor);
    ms_instanceProps.emplace_back("mass", GetMass, nullptr);
    ms_instanceProps.emplace_back("friction", GetFriction, SetFriction);
    ms_instanceProps.emplace_back("restitution", GetRestitution, SetRestitution);
    ms_instanceProps.emplace_back("motionType", GetMotionType, SetMotionType);

    ms_instanceMethods.emplace_back("applyForce", ApplyForce);
    ms_instanceMethods.emplace_back("applyCentralForce", ApplyCentralForce);
    ms_instanceMethods.emplace_back("applyImpulse", ApplyImpulse);
    ms_instanceMethods.emplace_back("applyCentralImpulse", ApplyCentralImpulse);
    ms_instanceMethods.emplace_back("applyTorque", ApplyTorque);

    ElementDefs::InheritTo(&ms_metaMethods, &ms_instanceProps);
    TransformableDefs::InheritTo(&ms_instanceProps);
    CollidableDefs::InheritTo(&ms_instanceMethods);
}
void ColliderDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_colliderName, Create, nullptr, nullptr, &ms_metaMethods, &ms_instanceProps, &ms_instanceMethods);
    p_vm->RegisterFunction("isCollider", IsCollider);
}

int ColliderDefs::Create(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::ICollider::ColliderType l_type;
    glm::vec3 *l_size;
    float l_mass = 1.f;
    l_argReader.Skip();
    l_argReader.ReadEnum(l_type, g_colliderTypes);
    l_argReader.ReadObject(l_size, g_vec3Name);
    l_argReader.ReadNextNumber(l_mass);
    if(!l_argReader.HasError())
    {
        ROC::ICollider *l_col = Core::GetInstance()->GetEngineCore()->GetIElementManager()->CreateICollider(l_type, *l_size, l_mass);
        if(l_col)
            l_argReader.PushObject(Utils::Cast<ROC::IElement*>(l_col), g_colliderName, true);
        else
            l_argReader.PushBoolean(false);
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int ColliderDefs::GetVelocity(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_colliderName);
    if(!l_argReader.HasError())
    {
        ROC::ICollider *l_col;
        if(Utils::IsValid(l_obj) && (l_col = Utils::Cast<ROC::ICollider*>(l_obj)))
        {
            glm::vec3 l_vel;
            l_col->GetVelocity(l_vel);
            l_argReader.PushObject(new glm::vec3(l_vel), g_vec3Name, false);
        }
        else
        {
            l_argReader.SetError(g_colliderInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int ColliderDefs::SetVelocity(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec3 *l_vec;
    l_argReader.ReadObject(l_obj, g_colliderName);
    l_argReader.ReadObject(l_vec, g_vec3Name);
    if(!l_argReader.HasError())
    {
        ROC::ICollider *l_col;
        if(Utils::IsValid(l_obj) && (l_col = Utils::Cast<ROC::ICollider*>(l_obj)))
            l_col->SetVelocity(*l_vec);
        else
            l_argReader.SetError(g_colliderInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int ColliderDefs::GetAngularVelocity(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_colliderName);
    if(!l_argReader.HasError())
    {
        ROC::ICollider *l_col;
        if(Utils::IsValid(l_obj) && (l_col = Utils::Cast<ROC::ICollider*>(l_obj)))
        {
            glm::vec3 l_vel;
            l_col->GetAngularVelocity(l_vel);
            l_argReader.PushObject(new glm::vec3(l_vel), g_vec3Name, false);
        }
        else
        {
            l_argReader.SetError(g_colliderInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int ColliderDefs::SetAngularVelocity(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec3 *l_vec;
    l_argReader.ReadObject(l_obj, g_colliderName);
    l_argReader.ReadObject(l_vec, g_vec3Name);
    if(!l_argReader.HasError())
    {
        ROC::ICollider *l_col;
        if(Utils::IsValid(l_obj) && (l_col = Utils::Cast<ROC::ICollider*>(l_obj)))
            l_col->SetAngularVelocity(*l_vec);
        else
            l_argReader.SetError(g_colliderInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int ColliderDefs::GetLinearFactor(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_colliderName);
    if(!l_argReader.HasError())
    {
        ROC::ICollider *l_col;
        if(Utils::IsValid(l_obj) && (l_col = Utils::Cast<ROC::ICollider*>(l_obj)))
        {
            glm::vec3 l_vel;
            l_col->GetLinearFactor(l_vel);
            l_argReader.PushObject(new glm::vec3(l_vel), g_vec3Name, false);
        }
        else
        {
            l_argReader.SetError(g_colliderInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int ColliderDefs::SetLinearFactor(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec3 *l_vec;
    l_argReader.ReadObject(l_obj, g_colliderName);
    l_argReader.ReadObject(l_vec, g_vec3Name);
    if(!l_argReader.HasError())
    {
        ROC::ICollider *l_col;
        if(Utils::IsValid(l_obj) && (l_col = Utils::Cast<ROC::ICollider*>(l_obj)))
            l_col->SetLinearFactor(*l_vec);
        else
            l_argReader.SetError(g_colliderInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int ColliderDefs::GetMass(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_colliderName);
    if(!l_argReader.HasError())
    {
        ROC::ICollider *l_col;
        if(Utils::IsValid(l_obj) && (l_col = Utils::Cast<ROC::ICollider*>(l_obj)))
            l_argReader.PushNumber(l_col->GetMass());
        else
        {
            l_argReader.SetError(g_colliderInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}

int ColliderDefs::GetFriction(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_colliderName);
    if(!l_argReader.HasError())
    {
        ROC::ICollider *l_col;
        if(Utils::IsValid(l_obj) && (l_col = Utils::Cast<ROC::ICollider*>(l_obj)))
            l_argReader.PushNumber(l_col->GetFriction());
        else
        {
            l_argReader.SetError(g_colliderInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int ColliderDefs::SetFriction(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    float l_val;
    l_argReader.ReadObject(l_obj, g_colliderName);
    l_argReader.ReadNumber(l_val);
    if(!l_argReader.HasError())
    {
        ROC::ICollider *l_col;
        if(Utils::IsValid(l_obj) && (l_col = Utils::Cast<ROC::ICollider*>(l_obj)))
            l_col->SetFriction(l_val);
        else
            l_argReader.SetError(g_colliderInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int ColliderDefs::GetRestitution(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_colliderName);
    if(!l_argReader.HasError())
    {
        ROC::ICollider *l_col;
        if(Utils::IsValid(l_obj) && (l_col = Utils::Cast<ROC::ICollider*>(l_obj)))
            l_argReader.PushNumber(l_col->GetRestitution());
        else
        {
            l_argReader.SetError(g_colliderInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int ColliderDefs::SetRestitution(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    float l_val;
    l_argReader.ReadObject(l_obj, g_colliderName);
    l_argReader.ReadNumber(l_val);
    if(!l_argReader.HasError())
    {
        ROC::ICollider *l_col;
        if(Utils::IsValid(l_obj) && (l_col = Utils::Cast<ROC::ICollider*>(l_obj)))
            l_col->SetRestitution(l_val);
        else
            l_argReader.SetError(g_colliderInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int ColliderDefs::GetMotionType(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    l_argReader.ReadObject(l_obj, g_colliderName);
    if(!l_argReader.HasError())
    {
        ROC::ICollider *l_col;
        if(Utils::IsValid(l_obj) && (l_col = Utils::Cast<ROC::ICollider*>(l_obj)))
            l_argReader.PushString(g_colliderMotionTypes[l_col->GetMotionType()]);
        else
        {
            l_argReader.SetError(g_colliderInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return 1;
}
int ColliderDefs::SetMotionType(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    ROC::ICollider::ColliderMotionType l_type;
    l_argReader.ReadObject(l_obj, g_colliderName);
    l_argReader.ReadEnum(l_type, g_colliderMotionTypes);
    if(!l_argReader.HasError())
    {
        ROC::ICollider *l_col;
        if(Utils::IsValid(l_obj) && (l_col = Utils::Cast<ROC::ICollider*>(l_obj)))
            l_col->SetMotionType(l_type);
        else
            l_argReader.SetError(g_colliderInvalid);
    }

    l_argReader.LogError();
    return 0;
}

int ColliderDefs::ApplyForce(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec3 *l_vecA;
    glm::vec3 *l_vecB;
    l_argReader.ReadObject(l_obj, g_colliderName);
    l_argReader.ReadObject(l_vecA, g_vec3Name);
    l_argReader.ReadObject(l_vecB, g_vec3Name);
    if(!l_argReader.HasError())
    {
        ROC::ICollider *l_col;
        if(Utils::IsValid(l_obj) && (l_col = Utils::Cast<ROC::ICollider*>(l_obj)))
        {
            l_col->ApplyForce(*l_vecA, *l_vecB);
            l_argReader.PushBoolean(true);
        }
        else
        {
            l_argReader.SetError(g_colliderInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int ColliderDefs::ApplyCentralForce(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec3 *l_vecA;
    l_argReader.ReadObject(l_obj, g_colliderName);
    l_argReader.ReadObject(l_vecA, g_vec3Name);
    if(!l_argReader.HasError())
    {
        ROC::ICollider *l_col;
        if(Utils::IsValid(l_obj) && (l_col = Utils::Cast<ROC::ICollider*>(l_obj)))
        {
            l_col->ApplyCentralForce(*l_vecA);
            l_argReader.PushBoolean(true);
        }
        else
        {
            l_argReader.SetError(g_colliderInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int ColliderDefs::ApplyImpulse(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec3 *l_vecA;
    glm::vec3 *l_vecB;
    l_argReader.ReadObject(l_obj, g_colliderName);
    l_argReader.ReadObject(l_vecA, g_vec3Name);
    l_argReader.ReadObject(l_vecB, g_vec3Name);
    if(!l_argReader.HasError())
    {
        ROC::ICollider *l_col;
        if(Utils::IsValid(l_obj) && (l_col = Utils::Cast<ROC::ICollider*>(l_obj)))
        {
            l_col->ApplyImpulse(*l_vecA, *l_vecB);
            l_argReader.PushBoolean(true);
        }
        else
        {
            l_argReader.SetError(g_colliderInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int ColliderDefs::ApplyCentralImpulse(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec3 *l_vecA;
    l_argReader.ReadObject(l_obj, g_colliderName);
    l_argReader.ReadObject(l_vecA, g_vec3Name);
    if(!l_argReader.HasError())
    {
        ROC::ICollider *l_col;
        if(Utils::IsValid(l_obj) && (l_col = Utils::Cast<ROC::ICollider*>(l_obj)))
        {
            l_col->ApplyCentralImpulse(*l_vecA);
            l_argReader.PushBoolean(true);
        }
        else
        {
            l_argReader.SetError(g_colliderInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int ColliderDefs::ApplyTorque(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj;
    glm::vec3 *l_vecA;
    bool l_impulse;
    l_argReader.ReadObject(l_obj, g_colliderName);
    l_argReader.ReadObject(l_vecA, g_vec3Name);
    l_argReader.ReadBoolean(l_impulse);
    if(!l_argReader.HasError())
    {
        ROC::ICollider *l_col;
        if(Utils::IsValid(l_obj) && (l_col = Utils::Cast<ROC::ICollider*>(l_obj)))
        {
            l_col->ApplyTorque(*l_vecA, l_impulse);
            l_argReader.PushBoolean(true);
        }
        else
        {
            l_argReader.SetError(g_colliderInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int ColliderDefs::IsCollider(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj = nullptr;
    l_argReader.ReadNextObject(l_obj, g_colliderName);
    l_argReader.PushBoolean(l_obj && Utils::IsValid(l_obj) && Utils::Cast<ROC::ICollider*>(l_obj));
    return l_argReader.GetReturnValue();
}

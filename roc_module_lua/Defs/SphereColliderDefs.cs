using LuaSharp.Lua;
using ROC.Engine.Objects.Components;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class SphereColliderDefs
    {
        static public readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();

        static SphereColliderDefs()
        {
            Definition.m_name = nameof(SphereCollider);

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),

                ("isValid", ComponentDefs.IsValid, null),
                ("type", ComponentDefs.GetComponentType, null),
                ("priority", ComponentDefs.GetPriority, null),
                ("enabled", ComponentDefs.GetEnabled, ComponentDefs.SetEnabled),
                ("gameObject", ComponentDefs.GetGameObject, null),

                ("angularFactor", ColliderDefs.GetAngularFactor, ColliderDefs.SetAngularFactor),
                ("angularVelocity", ColliderDefs.GetAngularVelocity, ColliderDefs.SetAngularVelocity),
                ("friction", ColliderDefs.GetFriction, ColliderDefs.SetFriction),
                ("linearFactor", ColliderDefs.GetLinearFactor, ColliderDefs.SetLinearFactor),
                ("mass", ColliderDefs.GetMass, ColliderDefs.SetMass),
                ("restitution", ColliderDefs.GetRestitution, ColliderDefs.SetRestitution),
                ("motionType", ColliderDefs.GetMotionType, ColliderDefs.SetMotionType),
                ("velocity", ColliderDefs.GetVelocity, ColliderDefs.SetVelocity),
                ("isActive", ColliderDefs.IsActive, null),

                ("radius", GetRadius, SetRadius)
            };

            Definition.m_instanceMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("Destroy", ComponentDefs.Destroy),

                ("ApplyCentralForce", ColliderDefs.ApplyCentralForce),
                ("ApplyCentralImpulse", ColliderDefs.ApplyCentralImpulse),
                ("ApplyForce", ColliderDefs.ApplyForce),
                ("ApplyImpulse", ColliderDefs.ApplyImpulse),
                ("ApplyTorque", ColliderDefs.ApplyTorque),
                ("ApplyTorqueImpulse", ColliderDefs.ApplyTorqueImpulse)
            };
        }

        static int GetRadius(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out SphereCollider l_col))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushNumber(l_col.Radius);
            return 1;
        }

        static int SetRadius(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out SphereCollider l_col) || !l_reader.ReadNumber(out float l_val))
                return 0;

            l_col.Radius = l_val;
            return 0;
        }
    }
}

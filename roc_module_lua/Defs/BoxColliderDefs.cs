using LuaSharp.Lua;
using ROC.Engine.Objects.Components;
using ROC.Module.Wrappers;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class BoxColliderDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_vector3Type = typeof(Vector3);

        static BoxColliderDefs()
        {
            Definition.m_name = nameof(BoxCollider);

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isValid", ObjectDefs.IsValid, null),

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

                ("size", GetSize, SetSize)
            };

            Definition.m_instanceMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("ApplyCentralForce", ColliderDefs.ApplyCentralForce),
                ("ApplyCentralImpulse", ColliderDefs.ApplyCentralImpulse),
                ("ApplyForce", ColliderDefs.ApplyForce),
                ("ApplyImpulse", ColliderDefs.ApplyImpulse),
                ("ApplyTorque", ColliderDefs.ApplyTorque),
                ("ApplyTorqueImpulse", ColliderDefs.ApplyTorqueImpulse),
                ("Activate", ColliderDefs.Activate)
            };
        }

        static int GetSize(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out BoxCollider l_col))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector3(l_col.Size), ms_vector3Type);
            return 1;
        }

        static int SetSize(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out BoxCollider l_col) || !l_argReader.ReadObject(out Vector3 l_val))
                return 0;

            l_col.Size = l_val.m_vector;
            return 0;
        }
    }
}

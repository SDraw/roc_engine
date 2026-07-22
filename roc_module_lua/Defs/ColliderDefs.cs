using LuaSharp.Lua;
using ROC.Engine.Objects.Components;
using ROC.Module.Wrappers;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class ColliderDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_vector3Type = typeof(Vector3);

        static ColliderDefs()
        {
            Definition.m_name = nameof(Collider);

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isValid", ObjectDefs.IsValid, null),

                ("type", ComponentDefs.GetComponentType, null),
                ("priority", ComponentDefs.GetPriority, null),
                ("enabled", ComponentDefs.GetEnabled, ComponentDefs.SetEnabled),
                ("gameObject", ComponentDefs.GetGameObject, null),

                ("angularFactor", GetAngularFactor, SetAngularFactor),
                ("angularVelocity", GetAngularVelocity, SetAngularVelocity),
                ("friction", GetFriction, SetFriction),
                ("linearFactor", GetLinearFactor, SetLinearFactor),
                ("mass", GetMass, SetMass),
                ("restitution", GetRestitution, SetRestitution),
                ("motionType", GetMotionType, SetMotionType),
                ("velocity", GetVelocity, SetVelocity),
                ("isActive", IsActive, null)
            };

            Definition.m_instanceMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("ApplyCentralForce", ApplyCentralForce),
                ("ApplyCentralImpulse", ApplyCentralImpulse),
                ("ApplyForce", ApplyForce),
                ("ApplyImpulse", ApplyImpulse),
                ("ApplyTorque", ApplyTorque),
                ("ApplyTorqueImpulse", ApplyTorqueImpulse),
                ("Activate", Activate)
            };
        }

        internal static int GetAngularFactor(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector3(l_col.AngularFactor), ms_vector3Type);
            return 1;
        }
        internal static int SetAngularFactor(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col) || !l_argReader.ReadObject(out Vector3 l_val))
                return 0;

            l_col.AngularFactor = l_val.m_vector;
            return 0;
        }

        internal static int GetAngularVelocity(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector3(l_col.AngularVelocity), ms_vector3Type);
            return 1;
        }
        internal static int SetAngularVelocity(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col) || !l_argReader.ReadObject(out Vector3 l_val))
                return 0;

            l_col.AngularVelocity = l_val.m_vector;
            return 0;
        }

        internal static int GetFriction(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_col.Friction);
            return 1;
        }
        internal static int SetFriction(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_col.Friction = l_val;
            return 0;
        }

        internal static int GetLinearFactor(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector3(l_col.LinearFactor), ms_vector3Type);
            return 1;
        }
        internal static int SetLinearFactor(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col) || !l_argReader.ReadObject(out Vector3 l_val))
                return 0;

            l_col.LinearFactor = l_val.m_vector;
            return 0;
        }

        internal static int GetMass(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_col.Mass);
            return 1;
        }
        internal static int SetMass(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_col.Mass = l_val;
            return 0;
        }

        internal static int GetRestitution(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_col.Restitution);
            return 1;
        }
        internal static int SetRestitution(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_col.Restitution = l_val;
            return 0;
        }

        internal static int GetMotionType(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushString(l_col.TypeOfMotion.ToString());
            return 1;
        }
        internal static int SetMotionType(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col) || !l_argReader.ReadEnum(out Collider.MotionType l_val))
                return 0;

            l_col.TypeOfMotion = l_val;
            return 0;
        }

        internal static int GetVelocity(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector3(l_col.Velocity), ms_vector3Type);
            return 1;
        }
        internal static int SetVelocity(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col) || !l_argReader.ReadObject(out Vector3 l_val))
                return 0;

            l_col.Velocity = l_val.m_vector;
            return 0;
        }

        internal static int IsActive(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushBoolean(l_col.IsActive);
            return 1;
        }

        internal static int ApplyCentralForce(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col) || !l_argReader.ReadObject(out Vector3 l_val))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_col.ApplyCentralForce(l_val.m_vector);
            l_argReader.PushBoolean(true);
            return 1;
        }

        internal static int ApplyCentralImpulse(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col) || !l_argReader.ReadObject(out Vector3 l_val))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_col.ApplyCentralImpulse(l_val.m_vector);
            l_argReader.PushBoolean(true);
            return 1;
        }

        internal static int ApplyForce(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col) || !l_argReader.ReadObject(out Vector3 l_valA) || !l_argReader.ReadObject(out Vector3 l_valB))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_col.ApplyForce(l_valA.m_vector, l_valB.m_vector);
            l_argReader.PushBoolean(true);
            return 1;
        }

        internal static int ApplyImpulse(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col) || !l_argReader.ReadObject(out Vector3 l_valA) || !l_argReader.ReadObject(out Vector3 l_valB))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_col.ApplyImpulse(l_valA.m_vector, l_valB.m_vector);
            l_argReader.PushBoolean(true);
            return 1;
        }

        internal static int ApplyTorque(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col) || !l_argReader.ReadObject(out Vector3 l_val))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_col.ApplyTorque(l_val.m_vector);
            l_argReader.PushBoolean(true);
            return 1;
        }

        internal static int ApplyTorqueImpulse(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col) || !l_argReader.ReadObject(out Vector3 l_val))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_col.ApplyTorqueImpulse(l_val.m_vector);
            l_argReader.PushBoolean(true);
            return 1;
        }

        internal static int Activate(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Collider l_col))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            bool l_force = false;
            if(l_argReader.IsNextBoolean())
                l_argReader.ReadBoolean(out l_force);

            l_col.Activate(l_force);
            l_argReader.PushBoolean(true);
            return 1;
        }
    }
}

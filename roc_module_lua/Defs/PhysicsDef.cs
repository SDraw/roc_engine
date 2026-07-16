using GlmSharp;
using LuaSharp.Lua;
using ROC.Module.Wrappers;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class PhysicsDef
    {
        static public readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static Type ms_vector3Type = typeof(Vector3);

        static PhysicsDef()
        {
            Definition.m_name = "Physics";

            Definition.m_staticProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("enabled", GetEnabled, SetEnabled),
                ("floor", GetFloor, SetFloor),
                ("gravity", GetGravity, SetGravity)
            };

            Definition.m_staticMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("Raycast", Raycast)
            };
        }

        static int GetEnabled(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            l_reader.PushBoolean(Engine.Core.Core.Instance.PhysicsManager.PhysicsEnabled);
            return 1;
        }
        static int SetEnabled(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadBoolean(out bool l_val))
                return 0;

            Engine.Core.Core.Instance.PhysicsManager.PhysicsEnabled = l_val;
            return 0;
        }

        static int GetFloor(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            l_reader.PushBoolean(Engine.Core.Core.Instance.PhysicsManager.FloorState);
            return 1;
        }
        static int SetFloor(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadBoolean(out bool l_val))
                return 0;

            Engine.Core.Core.Instance.PhysicsManager.FloorState = l_val;
            return 0;
        }

        static int GetGravity(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            l_reader.PushObject(new Vector3(Engine.Core.Core.Instance.PhysicsManager.Gravity), ms_vector3Type);
            return 1;
        }
        static int SetGravity(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Vector3 l_val))
                return 0;

            Engine.Core.Core.Instance.PhysicsManager.Gravity = l_val.m_vector;
            return 0;
        }

        static int Raycast(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Vector3 l_start) || !l_reader.ReadObject(out Vector3 l_end))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            if(!Engine.Core.Core.Instance.PhysicsManager.RayCast(l_start.m_vector, l_end.m_vector, out vec3 l_hit, out vec3 l_normal, out object l_obj))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushBoolean(true);
            l_reader.PushObject(new Vector3(l_hit), ms_vector3Type);
            l_reader.PushObject(new Vector3(l_normal), ms_vector3Type);
            if(l_obj != null)
                l_reader.PushObject(l_obj);
            else
                l_reader.PushBoolean(false);
            return 4;
        }
    }
}

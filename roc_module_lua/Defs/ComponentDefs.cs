using LuaSharp.Lua;
using ROC.Engine.Objects;
using ROC.Engine.Objects.Components;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class ComponentDefs
    {
        static public readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_gameObjectType = typeof(GameObject);

        static ComponentDefs()
        {
            Definition.m_name = nameof(Component);

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),

                ("isValid", IsValid, null),
                ("type", GetComponentType, null),
                ("priority", GetPriority, null),
                ("enabled", GetEnabled, SetEnabled),
                ("gameObject", GetGameObject, null),
            };

            Definition.m_instanceMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("Destroy", Destroy)
            };
        }

        internal static int IsValid(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Component l_component))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushBoolean(l_component.IsValid);
            return 1;
        }

        internal static int GetComponentType(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Component l_component))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushString(l_component.TypeOfComponent.ToString());
            return 1;
        }

        internal static int GetPriority(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Component l_component))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushInteger(l_component.ComponentPriority);
            return 1;
        }

        internal static int GetEnabled(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Component l_component))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushBoolean(l_component.Enabled);
            return 1;
        }
        internal static int SetEnabled(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Component l_component) || !l_reader.ReadBoolean(out bool l_val))
                return 0;

            l_component.Enabled = l_val;
            return 0;
        }

        internal static int GetGameObject(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Component l_component))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            if(l_component.GameObject != null)
                l_reader.PushObject(l_component.GameObject, ms_gameObjectType);
            else
                l_reader.PushBoolean(false);
            return 1;
        }

        internal static int Destroy(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Component l_component))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            Component.Destroy(l_component);
            l_reader.PushBoolean(true);
            return 1;
        }
    }
}

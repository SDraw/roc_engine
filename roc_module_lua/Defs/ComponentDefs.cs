using LuaSharp.Lua;
using ROC.Engine.Objects;
using ROC.Engine.Objects.Components;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class ComponentDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_gameObjectType = typeof(GameObject);

        static ComponentDefs()
        {
            Definition.m_name = nameof(Component);

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isValid", ObjectDefs.IsValid, null),

                ("type", GetComponentType, null),
                ("priority", GetPriority, null),
                ("enabled", GetEnabled, SetEnabled),
                ("gameObject", GetGameObject, null),
            };
        }

        internal static int GetComponentType(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Component l_component))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushString(l_component.TypeOfComponent.ToString());
            return 1;
        }

        internal static int GetPriority(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Component l_component))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushInteger(l_component.ComponentPriority);
            return 1;
        }

        internal static int GetEnabled(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Component l_component))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushBoolean(l_component.Enabled);
            return 1;
        }
        internal static int SetEnabled(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Component l_component) || !l_argReader.ReadBoolean(out bool l_val))
                return 0;

            l_component.Enabled = l_val;
            return 0;
        }

        internal static int GetGameObject(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Component l_component))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            if(l_component.GameObject != null)
                l_argReader.PushObject(l_component.GameObject, ms_gameObjectType);
            else
                l_argReader.PushBoolean(false);
            return 1;
        }
    }
}

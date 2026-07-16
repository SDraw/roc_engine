using LuaSharp.Lua;
using ROC.Engine.Objects.Components;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class MeshRendererDefs
    {
        static public readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();

        static MeshRendererDefs()
        {
            Definition.m_name = nameof(MeshRenderer);

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),

                ("isValid", ComponentDefs.IsValid, null),
                ("type", ComponentDefs.GetComponentType, null),
                ("priority", ComponentDefs.GetPriority, null),
                ("enabled", ComponentDefs.GetEnabled, ComponentDefs.SetEnabled),
                ("gameObject", ComponentDefs.GetGameObject, null),

                ("castShadows", GetCastShadows, SetCastShadows),
                ("recieveShadows", GetRecieveShadows, SetRecieveShadows)
            };

            Definition.m_instanceMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("Destroy", ComponentDefs.Destroy)
            };
        }

        static int GetCastShadows(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out MeshRenderer l_renderer))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushBoolean(l_renderer.CastShadows);
            return 1;
        }

        static int SetCastShadows(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out MeshRenderer l_renderer) || !l_reader.ReadBoolean(out bool l_val))
                return 0;

            l_renderer.CastShadows = l_val;
            return 0;
        }

        static int GetRecieveShadows(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out MeshRenderer l_renderer))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushBoolean(l_renderer.RecieveShadows);
            return 1;
        }

        static int SetRecieveShadows(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out MeshRenderer l_renderer) || !l_reader.ReadBoolean(out bool l_val))
                return 0;

            l_renderer.RecieveShadows = l_val;
            return 0;
        }
    }
}

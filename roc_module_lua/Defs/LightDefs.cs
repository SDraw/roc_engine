using LuaSharp.Lua;
using ROC.Engine.Objects.Components;
using ROC.Module.Wrappers;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class LightDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_vector2Type = typeof(Vector2);
        static readonly Type ms_vector3Type = typeof(Vector3);
        static readonly Type ms_vector4Type = typeof(Vector4);

        static LightDefs()
        {
            Definition.m_name = nameof(Light);

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isValid", ObjectDefs.IsValid, null),

                ("type", ComponentDefs.GetComponentType, null),
                ("priority", ComponentDefs.GetPriority, null),
                ("enabled", ComponentDefs.GetEnabled, ComponentDefs.SetEnabled),
                ("gameObject", ComponentDefs.GetGameObject, null),

                ("color", GetColor, SetColor),
                ("cutoff", GetCutoff, SetCutoff),
                ("falloff", GetFalloff, SetFalloff),
                ("priority", GetPriority, SetPriority),
                ("shadows", GetShadows, SetShadows),
                ("lightType", GetLightType, SetLightType),
            };
        }

        static int GetColor(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Light l_light))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector4(l_light.Color), ms_vector4Type);
            return 1;
        }
        static int SetColor(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Light l_light) || !l_argReader.ReadObject(out Vector4 l_val))
                return 0;

            l_light.Color = l_val.m_vector;
            return 0;
        }

        static int GetCutoff(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Light l_light))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector2(l_light.Cutoff), ms_vector2Type);
            return 1;
        }
        static int SetCutoff(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Light l_light) || !l_argReader.ReadObject(out Vector2 l_val))
                return 0;

            l_light.Cutoff = l_val.m_vector;
            return 0;
        }

        static int GetFalloff(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Light l_light))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector3(l_light.Falloff), ms_vector3Type);
            return 1;
        }
        static int SetFalloff(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Light l_light) || !l_argReader.ReadObject(out Vector3 l_val))
                return 0;

            l_light.Falloff = l_val.m_vector;
            return 0;
        }

        static int GetPriority(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Light l_light))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushInteger(l_light.Priority);
            return 1;
        }
        static int SetPriority(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Light l_light) || !l_argReader.ReadInteger(out int l_val))
                return 0;

            l_light.Priority = l_val;
            return 0;
        }

        static int GetShadows(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Light l_light))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushBoolean(l_light.Shadows);
            return 1;
        }
        static int SetShadows(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Light l_light) || !l_argReader.ReadBoolean(out bool l_val))
                return 0;

            l_light.Shadows = l_val;
            return 0;
        }

        static int GetLightType(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Light l_light))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushString(l_light.TypeOfLight.ToString());
            return 1;
        }
        static int SetLightType(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Light l_light) || !l_argReader.ReadEnum(out Light.LightType l_val))
                return 0;

            l_light.TypeOfLight = l_val;
            return 0;
        }
    }
}

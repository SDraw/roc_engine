using LuaSharp.Lua;
using ROC.Engine.Objects;
using ROC.Engine.Objects.Components;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class MeshRendererDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_materialType = typeof(Material);

        static MeshRendererDefs()
        {
            Definition.m_name = nameof(MeshRenderer);

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isValid", ObjectDefs.IsValid, null),

                ("type", ComponentDefs.GetComponentType, null),
                ("priority", ComponentDefs.GetPriority, null),
                ("enabled", ComponentDefs.GetEnabled, ComponentDefs.SetEnabled),
                ("gameObject", ComponentDefs.GetGameObject, null),

                ("castShadows", GetCastShadows, SetCastShadows),
                ("recieveShadows", GetRecieveShadows, SetRecieveShadows),
                ("materialsCount", GetMaterialsCount, null)
            };

            Definition.m_instanceMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("GetMaterial", GetMaterial),
                ("SetMaterial", SetMaterial)
            };
        }

        static int GetCastShadows(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out MeshRenderer l_renderer))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushBoolean(l_renderer.CastShadows);
            return 1;
        }

        static int SetCastShadows(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out MeshRenderer l_renderer) || !l_argReader.ReadBoolean(out bool l_val))
                return 0;

            l_renderer.CastShadows = l_val;
            return 0;
        }

        static int GetRecieveShadows(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out MeshRenderer l_renderer))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushBoolean(l_renderer.RecieveShadows);
            return 1;
        }

        static int SetRecieveShadows(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out MeshRenderer l_renderer) || !l_argReader.ReadBoolean(out bool l_val))
                return 0;

            l_renderer.RecieveShadows = l_val;
            return 0;
        }

        static int GetMaterial(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out MeshRenderer l_renderer) || !l_argReader.ReadInteger(out int l_index))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            var l_mat = l_renderer.GetMaterial(l_index);
            if(l_mat != null)
                l_argReader.PushObject(l_mat, ms_materialType);
            else
                l_argReader.PushBoolean(false);
            return 1;
        }

        static int GetMaterialsCount(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out MeshRenderer l_renderer))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushInteger(l_renderer.MaterialsCount);
            return 1;
        }

        static int SetMaterial(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out MeshRenderer l_renderer) || !l_argReader.ReadInteger(out int l_index))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            if(l_argReader.IsNextNil())
            {
                l_renderer.SetMaterial(l_index, null);
                l_argReader.PushBoolean(true);
                return 1;
            }

            if(!l_argReader.ReadObject(out Material l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_renderer.SetMaterial(l_index, l_mat);
            l_argReader.PushBoolean(true);
            return 1;
        }
    }
}

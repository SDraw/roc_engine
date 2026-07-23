using LuaSharp.Lua;
using ROC.Engine.Objects;
using ROC.Engine.Objects.Resources;
using ROC.Module.Wrappers;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class MaterialDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_materialType = typeof(Material);
        static readonly Type ms_textureType = typeof(Texture);
        static readonly Type ms_vector4Type = typeof(Vector4);

        static MaterialDefs()
        {
            Definition.m_name = nameof(Material);
            Definition.m_constructor = Create;

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isValid", ObjectDefs.IsValid, null),

                ("renderMode", GetRenderMode, SetRenderMode),
                ("doubleSided", GetDoubleSided, SetDoubleSided),
                ("unlit", GetUnlit, SetUnlit),
                ("color", GetColor, SetColor),
                ("parameters", GetParameters, SetParameters),
                ("diffuseTexture", GetDiffuseTexture, SetDiffuseTexture)
            };
        }

        static int Create(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.PushObject(Material.Create(), ms_materialType);
            return 1;
        }

        static int GetRenderMode(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Material l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushString(l_mat.Mode.ToString());
            return 1;
        }
        static int SetRenderMode(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Material l_mat) || !l_argReader.ReadEnum(out Material.RenderMode l_val))
                return 0;

            l_mat.Mode = l_val;
            return 0;
        }

        static int GetDoubleSided(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Material l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushBoolean(l_mat.DoubleSided);
            return 1;
        }
        static int SetDoubleSided(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Material l_mat) || !l_argReader.ReadBoolean(out bool l_val))
                return 0;

            l_mat.DoubleSided = l_val;
            return 0;
        }

        static int GetUnlit(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Material l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushBoolean(l_mat.Unlit);
            return 1;
        }
        static int SetUnlit(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Material l_mat) || !l_argReader.ReadBoolean(out bool l_val))
                return 0;

            l_mat.Unlit = l_val;
            return 0;
        }

        static int GetColor(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Material l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector4(l_mat.Color), ms_vector4Type);
            return 1;
        }
        static int SetColor(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Material l_mat) || !l_argReader.ReadObject(out Vector4 l_val))
                return 0;

            l_mat.Color = l_val.m_vector;
            return 0;
        }

        static int GetParameters(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Material l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector4(l_mat.Params), ms_vector4Type);
            return 1;
        }
        static int SetParameters(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Material l_mat) || !l_argReader.ReadObject(out Vector4 l_val))
                return 0;

            l_mat.Params = l_val.m_vector;
            return 0;
        }

        static int GetDiffuseTexture(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Material l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            if(l_mat.DiffuseTexture != null)
                l_argReader.PushObject(l_mat.DiffuseTexture, ms_textureType);
            else
                l_argReader.PushBoolean(false);
            return 1;
        }
        static int SetDiffuseTexture(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Material l_mat))
                return 0;

            if(l_argReader.IsNextNil())
            {
                l_mat.DiffuseTexture = null;
                return 0;
            }

            if(!l_argReader.ReadObject(out Texture l_tex))
                return 0;

            l_mat.DiffuseTexture = l_tex;
            return 0;
        }
    }
}

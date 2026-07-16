using LuaSharp.Lua;
using ROC.Engine.Objects.Resources;
using ROC.Module.Wrappers;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class ShaderDefs
    {
        static public readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_shaderType = typeof(Shader);

        static ShaderDefs()
        {
            Definition.m_name = nameof(Shader);
            Definition.m_constructor = Create;

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isLoaded", ResourceDefs.IsLoaded, null),
                ("log", ResourceDefs.Log, null)
            };

            Definition.m_instanceMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("Unload", ResourceDefs.Unload),

                ("SetValue", SetValue)
            };
        }

        static int Create(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            l_reader.Skip();

            if(!l_reader.ReadString(out string l_vert) || !l_reader.ReadString(out string l_frag))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushObject(Shader.Import(l_vert, l_frag), ms_shaderType);
            return 1;
        }

        static int SetValue(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Shader l_shader) || !l_reader.ReadString(out string l_uniform) || !l_reader.ReadValue(out object p_value))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            switch(p_value)
            {
                case double d:
                    l_shader.SetValue(l_uniform, (float)d);
                    break;

                case long i:
                    l_shader.SetValue(l_uniform, (int)i);
                    break;

                case Vector2 v2:
                    l_shader.SetValue(l_uniform, v2.m_vector);
                    break;

                case Vector3 v3:
                    l_shader.SetValue(l_uniform, v3.m_vector);
                    break;

                case Vector4 v4:
                    l_shader.SetValue(l_uniform, v4.m_vector);
                    break;

                case Matrix2 m2:
                    l_shader.SetValue(l_uniform, m2.m_matrix);
                    break;

                case Matrix3 m3:
                    l_shader.SetValue(l_uniform, m3.m_matrix);
                    break;

                case Matrix4 m4:
                    l_shader.SetValue(l_uniform, m4.m_matrix);
                    break;
            }

            l_reader.PushBoolean(true);
            return 1;
        }
    }
}

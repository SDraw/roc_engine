using GlmSharp;
using OpenGL;
using ROC.Engine.OGL;
using System;
using System.Collections.Generic;
using System.IO;

namespace ROC.Engine.Objects.Resources
{
    public sealed class Shader : Resource
    {
        struct ShaderUniform
        {
            public int m_location;
            public int m_internalType;
        }

        static readonly (string, int)[] ms_defaultUniforms = new (string, int)[]
        {
            ("gProjectionMatrix", Gl.FLOAT_MAT4),
            ("gViewMatrix", Gl.FLOAT_MAT4),
            ("gViewProjectionMatrix", Gl.FLOAT_MAT4),
            ("gModelMatrix", Gl.FLOAT_MAT4),
            ("gAnimated", Gl.BOOL),
            ("gBoneMatrix", Gl.FLOAT_MAT4),
            ("gCameraPosition", Gl.FLOAT_VEC3),
            ("gCameraDirection", Gl.FLOAT_VEC3),
            ("gShadowViewProjectionMatrix", Gl.FLOAT_MAT4),
            ("gLightData", Gl.FLOAT_MAT4),
            ("gLightsCount", Gl.INT),
            ("gMaterialParam", Gl.FLOAT_VEC4),
            ("gMaterialType", Gl.BOOL_VEC4),
            ("gTexture0", Gl.SAMPLER_2D),
            ("gTextureShadow", Gl.SAMPLER_2D_SHADOW),
            ("gColor", Gl.FLOAT_VEC4),
            ("gTime", Gl.FLOAT_VEC4)
        };

        GLShader m_glShader = null;

        readonly Dictionary<string, ShaderUniform> m_uniforms = null;

        internal Shader()
        {
            m_glShader = null;
            m_loaded = false;
            m_uniforms = new Dictionary<string, ShaderUniform>();
        }

        internal void Load(string p_vPath, string p_fPath, bool p_source = false)
        {
            if(m_loaded)
                return;

            m_glShader = new GLShader();
            try
            {
                m_loaded = m_glShader.Create(p_source ? p_vPath : File.ReadAllText(p_vPath), p_source ? p_fPath : File.ReadAllText(p_fPath));
                if(m_loaded)
                {
                    ParseUniforms();
                    SetDefaultValues();
                }
                else
                {
                    m_log = m_glShader.Log;
                    m_glShader = null;
                }
            }
            catch(Exception e)
            {
                m_log = e.Message;
            }
        }

        // Resource
        public override void Unload()
        {
            if(!m_loaded)
                return;

            m_uniforms.Clear();
            m_log = "";

            m_glShader.Destroy();
            m_glShader = null;

            m_loaded = false;
        }

        // Init
        void ParseUniforms()
        {
            foreach(var l_pair in ms_defaultUniforms)
            {
                int l_location = m_glShader.GetUniformLocation(l_pair.Item1);
                if(l_location != -1)
                {
                    m_uniforms.Add(l_pair.Item1, new ShaderUniform()
                    {
                        m_location = l_location,
                        m_internalType = l_pair.Item2
                    });
                }
            }

            for(int i = 0, j = m_glShader.GetUniformsCount(); i < j; i++)
            {
                m_glShader.GetUniformInfo(i, out var l_type, out string l_name);

                if(l_name.Length > 0 && !m_uniforms.ContainsKey(l_name))
                {
                    m_uniforms.Add(l_name, new ShaderUniform()
                    {
                        m_location = m_glShader.GetUniformLocation(l_name),
                        m_internalType = l_type
                    });
                }
            }
        }

        void SetDefaultValues()
        {
            m_glShader.SetAttribute(0, "gVertexPosition");
            m_glShader.SetAttribute(1, "gVertexNormal");
            m_glShader.SetAttribute(2, "gVertexUV");
            m_glShader.SetAttribute(3, "gVertexWeight");
            m_glShader.SetAttribute(4, "gVertexIndex");

            if(m_uniforms.TryGetValue("gTexture0", out var l_value) && l_value.m_internalType == Gl.SAMPLER_2D)
                m_glShader.SetUniformValue(l_value.m_location, 0); // TextureUnit 0

            if(m_uniforms.TryGetValue("gTextureShadow", out l_value) && l_value.m_internalType == Gl.SAMPLER_2D_SHADOW)
                m_glShader.SetUniformValue(l_value.m_location, 1); // TextureUnit 1
        }

        // Internal usage
        internal void SetProjection(mat4 p_mat)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue("gProjectionMatrix", out var l_value) && l_value.m_internalType == Gl.FLOAT_MAT4)
                m_glShader.SetUniformValue(l_value.m_location, p_mat);
        }

        internal void SetView(mat4 p_mat)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue("gViewMatrix", out var l_value) && l_value.m_internalType == Gl.FLOAT_MAT4)
                m_glShader.SetUniformValue(l_value.m_location, p_mat);
        }

        internal void SetViewProjection(mat4 p_mat)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue("gViewProjectionMatrix", out var l_value) && l_value.m_internalType == Gl.FLOAT_MAT4)
                m_glShader.SetUniformValue(l_value.m_location, p_mat);
        }

        internal void SetShadowViewProjection(mat4 p_mat)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue("gShadowViewProjectionMatrix", out var l_value) && l_value.m_internalType == Gl.FLOAT_MAT4)
                m_glShader.SetUniformValue(l_value.m_location, p_mat);
        }

        internal void SetModel(mat4 p_mat)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue("gModelMatrix", out var l_value) && l_value.m_internalType == Gl.FLOAT_MAT4)
                m_glShader.SetUniformValue(l_value.m_location, p_mat);
        }

        internal void SetAnimated(bool p_state)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue("gAnimated", out var l_value) && l_value.m_internalType == Gl.BOOL)
                m_glShader.SetUniformValue(l_value.m_location, p_state ? 1 : 0);
        }

        internal void SetBones(mat4[] p_mat)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue("gBoneMatrix", out var l_value) && l_value.m_internalType == Gl.FLOAT_MAT4)
                m_glShader.SetUniformValue(l_value.m_location, p_mat);
        }

        internal void SetCamera(vec3 p_pos, vec3 p_dir)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue("gCameraPosition", out var l_value) && l_value.m_internalType == Gl.FLOAT_VEC3)
                m_glShader.SetUniformValue(l_value.m_location, p_pos.x, p_pos.y, p_pos.z);

            if(m_uniforms.TryGetValue("gCameraDirection", out l_value) && l_value.m_internalType == Gl.FLOAT_VEC3)
                m_glShader.SetUniformValue(l_value.m_location, p_dir.x, p_dir.y, p_dir.z);
        }

        internal void SetLights(mat4[] p_mat, int p_count)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue("gLightData", out var l_value) && l_value.m_internalType == Gl.FLOAT_MAT4)
                m_glShader.SetUniformValue(l_value.m_location, p_mat);

            if(m_uniforms.TryGetValue("gLightsCount", out l_value) && l_value.m_internalType == Gl.INT)
                m_glShader.SetUniformValue(l_value.m_location, p_count);
        }

        internal void SetMaterial(bvec4 p_type, vec4 p_params)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue("gMaterialType", out var l_value) && l_value.m_internalType == Gl.BOOL_VEC4)
                m_glShader.SetUniformValue(l_value.m_location, p_type.x ? 1 : 0, p_type.y ? 1 : 0, p_type.z ? 1 : 0, p_type.w ? 1 : 0);

            if(m_uniforms.TryGetValue("gMaterialParam", out l_value) && l_value.m_internalType == Gl.FLOAT_VEC3)
                m_glShader.SetUniformValue(l_value.m_location, p_params.x, p_params.y, p_params.z, p_params.w);
        }

        internal void SetColor(vec4 p_color)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue("gColor", out var l_value) && l_value.m_internalType == Gl.FLOAT_VEC4)
                m_glShader.SetUniformValue(l_value.m_location, p_color.r, p_color.g, p_color.b, p_color.a);
        }

        internal void SetTime(float p_time)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue("gTime", out var l_value) && l_value.m_internalType == Gl.FLOAT)
                m_glShader.SetUniformValue(l_value.m_location, p_time);
        }

        // Public values set
        public void SetValue(string p_uniform, object p_value)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue(p_uniform, out var l_uniform))
            {
                switch(l_uniform.m_internalType)
                {
                    case Gl.FLOAT:
                    {
                        if(p_value is float l_value)
                            m_glShader.SetUniformValue(l_uniform.m_location, l_value);
                    }
                    break;

                    case Gl.FLOAT_VEC2:
                    {
                        if(p_value is vec2 l_value)
                            m_glShader.SetUniformValue(l_uniform.m_location, l_value.x, l_value.y);
                    }
                    break;

                    case Gl.FLOAT_VEC3:
                    {
                        if(p_value is vec3 l_value)
                            m_glShader.SetUniformValue(l_uniform.m_location, l_value.x, l_value.y, l_value.z);
                    }
                    break;

                    case Gl.FLOAT_VEC4:
                    {
                        if(p_value is vec4 l_value)
                            m_glShader.SetUniformValue(l_uniform.m_location, l_value.x, l_value.y, l_value.z, l_value.w);
                    }
                    break;

                    case Gl.FLOAT_MAT2:
                    {
                        if(p_value is mat2 l_value)
                            m_glShader.SetUniformValue(l_uniform.m_location, l_value);
                    }
                    break;

                    case Gl.FLOAT_MAT3:
                    {
                        if(p_value is mat3 l_value)
                            m_glShader.SetUniformValue(l_uniform.m_location, l_value);
                    }
                    break;

                    case Gl.FLOAT_MAT4:
                    {
                        if(p_value is mat4 l_value)
                            m_glShader.SetUniformValue(l_uniform.m_location, l_value);
                    }
                    break;

                    case Gl.INT:
                    case Gl.SAMPLER_2D:
                    case Gl.SAMPLER_2D_SHADOW:
                    {
                        if(p_value is int l_value)
                            m_glShader.SetUniformValue(l_uniform.m_location, l_value);
                    }
                    break;

                    case Gl.INT_VEC2:
                    {
                        if(p_value is ivec2 l_value)
                            m_glShader.SetUniformValue(l_uniform.m_location, l_value.x, l_value.y);
                    }
                    break;

                    case Gl.INT_VEC3:
                    {
                        if(p_value is ivec3 l_value)
                            m_glShader.SetUniformValue(l_uniform.m_location, l_value.x, l_value.y, l_value.z);
                    }
                    break;

                    case Gl.INT_VEC4:
                    {
                        if(p_value is ivec4 l_value)
                            m_glShader.SetUniformValue(l_uniform.m_location, l_value.x, l_value.y, l_value.z, l_value.w);
                    }
                    break;


                    case Gl.UNSIGNED_INT:
                    {
                        if(p_value is uint l_value)
                            m_glShader.SetUniformValue(l_uniform.m_location, l_value);
                    }
                    break;

                    case Gl.UNSIGNED_INT_VEC2:
                    {
                        if(p_value is uvec2 l_value)
                            m_glShader.SetUniformValue(l_uniform.m_location, l_value.x, l_value.y);
                    }
                    break;

                    case Gl.UNSIGNED_INT_VEC3:
                    {
                        if(p_value is uvec3 l_value)
                            m_glShader.SetUniformValue(l_uniform.m_location, l_value.x, l_value.y, l_value.z);
                    }
                    break;

                    case Gl.UNSIGNED_INT_VEC4:
                    {
                        if(p_value is uvec4 l_value)
                            m_glShader.SetUniformValue(l_uniform.m_location, l_value.x, l_value.y, l_value.z, l_value.w);
                    }
                    break;
                }
            }
        }

        internal void Activate()
        {
            if(!IsLoaded)
                return;

            m_glShader.Bind();
        }

        // API
        public static Shader Import(string p_vert, string p_frag)
        {
            Shader l_shader = new Shader();
            l_shader.Load(p_vert, p_frag);
            return l_shader;
        }
    }
}

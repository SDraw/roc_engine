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

        const string c_projectionMatrix = "gProjectionMatrix";
        const string c_viewMatrix = "gViewMatrix";
        const string c_viewProjectionMatrix = "gViewProjectionMatrix";
        const string c_modelMatrix = "gModelMatrix";
        const string c_animated = "gAnimated";
        const string c_boneMatrix = "gBoneMatrix";
        const string c_cameraPosition = "gCameraPosition";
        const string c_cameraDirection = "gCameraDirection";
        const string c_shadowViewProjection = "gShadowViewProjectionMatrix";
        const string c_lightLightsCount = "gLightsCount";
        const string c_lightData = "gLightData";
        const string c_materialType = "gMaterialType";
        const string c_materialColor = "gMaterialColor";
        const string c_materialParams = "gMaterialParams";
        const string c_textureDiffuse = "gTexture0";
        const string c_textureShadow = "gTextureShadow";
        const string c_time = "gTime";

        static readonly (string, int)[] ms_defaultUniforms = new (string, int)[]
        {
            (c_projectionMatrix, Gl.FLOAT_MAT4),
            (c_viewMatrix, Gl.FLOAT_MAT4),
            (c_viewProjectionMatrix, Gl.FLOAT_MAT4),
            (c_modelMatrix, Gl.FLOAT_MAT4),
            (c_animated, Gl.BOOL),
            (c_boneMatrix, Gl.FLOAT_MAT4),
            (c_cameraPosition, Gl.FLOAT_VEC3),
            (c_cameraDirection, Gl.FLOAT_VEC3),
            (c_shadowViewProjection, Gl.FLOAT_MAT4),
            (c_lightLightsCount, Gl.INT),
            (c_lightData, Gl.FLOAT_MAT4),
            (c_materialType, Gl.UNSIGNED_INT_VEC4),
            (c_materialColor, Gl.FLOAT_VEC4),
            (c_materialParams, Gl.FLOAT_VEC4),
            (c_textureDiffuse, Gl.SAMPLER_2D),
            (c_textureShadow, Gl.SAMPLER_2D_SHADOW),
            (c_time, Gl.FLOAT)
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
        protected override void DestroyInternal()
        {
            if(m_loaded)
            {
                m_uniforms.Clear();
                m_log = "";

                m_glShader.Destroy();
                m_glShader = null;

                m_loaded = false;
            }

            base.DestroyInternal();
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

            if(m_uniforms.TryGetValue(c_textureDiffuse, out var l_value) && l_value.m_internalType == Gl.SAMPLER_2D)
                m_glShader.SetUniformValue(l_value.m_location, 0); // TextureUnit 0

            if(m_uniforms.TryGetValue(c_textureShadow, out l_value) && l_value.m_internalType == Gl.SAMPLER_2D_SHADOW)
                m_glShader.SetUniformValue(l_value.m_location, 1); // TextureUnit 1
        }

        // Internal usage
        internal void SetProjection(mat4 p_mat)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue(c_projectionMatrix, out var l_value) && l_value.m_internalType == Gl.FLOAT_MAT4)
                m_glShader.SetUniformValue(l_value.m_location, p_mat);
        }

        internal void SetView(mat4 p_mat)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue(c_viewMatrix, out var l_value) && l_value.m_internalType == Gl.FLOAT_MAT4)
                m_glShader.SetUniformValue(l_value.m_location, p_mat);
        }

        internal void SetViewProjection(mat4 p_mat)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue(c_viewProjectionMatrix, out var l_value) && l_value.m_internalType == Gl.FLOAT_MAT4)
                m_glShader.SetUniformValue(l_value.m_location, p_mat);
        }

        internal void SetShadowViewProjection(mat4 p_mat)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue(c_shadowViewProjection, out var l_value) && l_value.m_internalType == Gl.FLOAT_MAT4)
                m_glShader.SetUniformValue(l_value.m_location, p_mat);
        }

        internal void SetModel(mat4 p_mat)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue(c_modelMatrix, out var l_value) && l_value.m_internalType == Gl.FLOAT_MAT4)
                m_glShader.SetUniformValue(l_value.m_location, p_mat);
        }

        internal void SetAnimated(bool p_state)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue(c_animated, out var l_value) && l_value.m_internalType == Gl.BOOL)
                m_glShader.SetUniformValue(l_value.m_location, p_state ? 1 : 0);
        }

        internal void SetBones(mat4[] p_mat)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue(c_boneMatrix, out var l_value) && l_value.m_internalType == Gl.FLOAT_MAT4)
                m_glShader.SetUniformValue(l_value.m_location, p_mat);
        }

        internal void SetCamera(vec3 p_pos, vec3 p_dir)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue(c_cameraPosition, out var l_value) && l_value.m_internalType == Gl.FLOAT_VEC3)
                m_glShader.SetUniformValue(l_value.m_location, p_pos.x, p_pos.y, p_pos.z);

            if(m_uniforms.TryGetValue(c_cameraDirection, out l_value) && l_value.m_internalType == Gl.FLOAT_VEC3)
                m_glShader.SetUniformValue(l_value.m_location, p_dir.x, p_dir.y, p_dir.z);
        }

        internal void SetLights(mat4[] p_mat, int p_count)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue(c_lightLightsCount, out var l_value) && l_value.m_internalType == Gl.INT)
                m_glShader.SetUniformValue(l_value.m_location, p_count);

            if(m_uniforms.TryGetValue(c_lightData, out l_value) && l_value.m_internalType == Gl.FLOAT_MAT4)
                m_glShader.SetUniformValue(l_value.m_location, p_mat);
        }

        internal void SetMaterial(uvec4 p_type, vec4 p_color, vec4 p_params)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue(c_materialType, out var l_value) && l_value.m_internalType == Gl.UNSIGNED_INT_VEC4)
                m_glShader.SetUniformValue(l_value.m_location, p_type.x, p_type.y, p_type.z, p_type.w);

            if(m_uniforms.TryGetValue(c_materialColor, out l_value) && l_value.m_internalType == Gl.FLOAT_VEC4)
                m_glShader.SetUniformValue(l_value.m_location, p_color.r, p_color.g, p_color.b, p_color.a);

            if(m_uniforms.TryGetValue(c_materialParams, out l_value) && l_value.m_internalType == Gl.FLOAT_VEC4)
                m_glShader.SetUniformValue(l_value.m_location, p_params.x, p_params.y, p_params.z, p_params.w);
        }

        internal void SetTime(float p_time)
        {
            if(!m_loaded)
                return;

            if(m_uniforms.TryGetValue(c_time, out var l_value) && l_value.m_internalType == Gl.FLOAT)
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
            if(!m_loaded)
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

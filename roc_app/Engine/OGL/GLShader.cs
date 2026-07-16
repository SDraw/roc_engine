using GlmSharp;
using OpenGL;
using System;
using System.Text;

namespace ROC.Engine.OGL
{
    internal sealed class GLShader
    {
        [ThreadStatic]
        static uint ms_lastProgram = 0U;

        uint m_program = 0U;
        string m_log;

        public GLShader()
        {
            m_program = 0U;
        }

        public bool IsValid => (m_program != 0U);

        public bool Create(string p_vertex, string p_fragment)
        {
            if(m_program != 0U)
                return false;

            uint l_vertexShader = Gl.CreateShader(ShaderType.VertexShader);
            if(l_vertexShader != 0)
            {
                Gl.ShaderSource(l_vertexShader, new string[1] { p_vertex });
                Gl.CompileShader(l_vertexShader);
                Gl.GetShader(l_vertexShader, ShaderParameterName.CompileStatus, out int l_state);

                if(l_state == 0)
                {
                    StringBuilder l_builder = new StringBuilder(1024);
                    Gl.GetShaderInfoLog(l_vertexShader, 1024, out _, l_builder);
                    m_log += "Vertex shader error: " + l_builder.ToString();
                }
            }

            uint l_fragmentShader = Gl.CreateShader(ShaderType.FragmentShader);
            if(l_fragmentShader != 0U)
            {
                Gl.ShaderSource(l_fragmentShader, new string[1] { p_fragment });
                Gl.CompileShader(l_fragmentShader);
                Gl.GetShader(l_fragmentShader, ShaderParameterName.CompileStatus, out int l_state);

                if(l_state == 0)
                {
                    StringBuilder l_builder = new StringBuilder(1024);
                    Gl.GetShaderInfoLog(l_fragmentShader, 1024, out _, l_builder);
                    m_log += "Fragment shader error: " + l_builder.ToString();
                }
            }

            if((l_vertexShader != 0) && (l_fragmentShader != 0U))
            {
                m_program = Gl.CreateProgram();
                if(m_program != 0U)
                {
                    Gl.AttachShader(m_program, l_vertexShader);
                    Gl.AttachShader(m_program, l_fragmentShader);
                    Gl.LinkProgram(m_program);

                    Gl.GetProgram(m_program, ProgramProperty.LinkStatus, out int l_link);
                    if(l_link == 0)
                    {
                        StringBuilder l_builder = new StringBuilder(1024);
                        Gl.GetProgramInfoLog(m_program, 1024, out _, l_builder);
                        m_log += "Shader program error: " + l_builder.ToString();

                        Gl.DeleteProgram(m_program);
                        m_program = 0;
                    }
                    else
                    {
                        Gl.DetachShader(m_program, l_vertexShader);
                        Gl.DetachShader(m_program, l_fragmentShader);
                    }
                }
            }

            if(l_vertexShader != 0)
                Gl.DeleteShader(l_vertexShader);
            if(l_fragmentShader != 0)
                Gl.DeleteShader(l_fragmentShader);

            return (m_program != 0);
        }

        public bool Destroy()
        {
            if(m_program == 0U)
                return false;

            if(ms_lastProgram == m_program)
                ms_lastProgram = 0;

            Gl.DeleteProgram(m_program);
            m_program = 0;

            return true;
        }

        public string Log => m_log;

        public void Bind()
        {
            if(m_program == 0U)
                return;

            if(ms_lastProgram == m_program)
                return;

            Gl.UseProgram(m_program);
            ms_lastProgram = m_program;
        }

        // Uniforms
        public int GetUniformsCount()
        {
            if(m_program == 0U)
                return 0;

            Gl.GetProgram(m_program, ProgramProperty.ActiveUniforms, out int l_count);
            return l_count;
        }

        public int GetUniformLocation(string p_name)
        {
            if(m_program == 0U)
                return -1;

            return Gl.GetUniformLocation(m_program, p_name);
        }

        public void GetUniformInfo(int p_index, out int p_type, out string p_name)
        {
            p_type = 0;
            p_name = "";

            if(m_program == 0U)
                return;

            StringBuilder l_builder = new StringBuilder(1024);
            Gl.GetActiveUniform(m_program, (uint)p_index, 1024, out _, out _, out p_type, l_builder);
            p_name = l_builder.ToString();
        }

        // Attributes
        public bool SetAttribute(int p_index, string p_name)
        {
            if(m_program == 0U)
                return false;

            Gl.BindAttribLocation(m_program, (uint)p_index, p_name);
            return true;
        }

        // Float and float vectors
        public void SetUniformValue(int p_location, float p_v0)
        {
            if(m_program == 0U)
                return;

            Bind();
            Gl.Uniform1(p_location, p_v0);
        }

        public void SetUniformValue(int p_location, float p_v0, float p_v1)
        {
            if(m_program == 0U)
                return;

            Bind();
            Gl.Uniform2(p_location, p_v0, p_v1);
        }

        public void SetUniformValue(int p_location, float p_v0, float p_v1, float p_v2)
        {
            if(m_program == 0U)
                return;

            Bind();
            Gl.Uniform3(p_location, p_v0, p_v1, p_v2);
        }

        public void SetUniformValue(int p_location, float p_v0, float p_v1, float p_v2, float p_v3)
        {
            if(m_program == 0U)
                return;

            Bind();
            Gl.Uniform4(p_location, p_v0, p_v1, p_v2, p_v3);
        }

        // Ints and int vectors
        public void SetUniformValue(int p_location, int p_v0)
        {
            if(m_program == 0U)
                return;

            Bind();
            Gl.Uniform1(p_location, p_v0);
        }

        public void SetUniformValue(int p_location, int p_v0, int p_v1)
        {
            if(m_program == 0U)
                return;

            Bind();
            Gl.Uniform2(p_location, p_v0, p_v1);
        }

        public void SetUniformValue(int p_location, int p_v0, int p_v1, int p_v2)
        {
            if(m_program == 0U)
                return;

            Bind();
            Gl.Uniform3(p_location, p_v0, p_v1, p_v2);
        }

        public void SetUniformValue(int p_location, int p_v0, int p_v1, int p_v2, int p_v3)
        {
            if(m_program == 0U)
                return;

            Bind();
            Gl.Uniform4(p_location, p_v0, p_v1, p_v2, p_v3);
        }

        // Unsigned ints and uints vectors
        public void SetUniformValue(int p_location, uint p_v0)
        {
            if(m_program == 0U)
                return;

            Bind();
            Gl.Uniform1(p_location, p_v0);
        }

        public void SetUniformValue(int p_location, uint p_v0, uint p_v1)
        {
            if(m_program == 0U)
                return;

            Bind();
            Gl.Uniform2(p_location, p_v0, p_v1);
        }

        public void SetUniformValue(int p_location, uint p_v0, uint p_v1, uint p_v2)
        {
            if(m_program == 0U)
                return;

            Bind();
            Gl.Uniform3(p_location, p_v0, p_v1, p_v2);
        }

        public void SetUniformValue(int p_location, uint p_v0, uint p_v1, uint p_v2, uint p_v3)
        {
            if(m_program == 0U)
                return;

            Bind();
            Gl.Uniform4(p_location, p_v0, p_v1, p_v2, p_v3);
        }

        // Matrices and matrix arrays
        public void SetUniformValue(int p_location, mat2 p_value)
        {
            if(m_program == 0U)
                return;

            Bind();
            unsafe
            {
                Gl.UniformMatrix2(p_location, 1, false, &p_value.m00);
            }
        }

        public void SetUniformValue(int p_location, mat3 p_value)
        {
            if(m_program == 0U)
                return;

            Bind();
            unsafe
            {
                Gl.UniformMatrix3(p_location, 1, false, &p_value.m00);
            }
        }

        public void SetUniformValue(int p_location, mat4 p_value)
        {
            if(m_program == 0U)
                return;

            Bind();
            unsafe
            {
                Gl.UniformMatrix4(p_location, 1, false, &p_value.m00);
            }
        }

        public void SetUniformValue(int p_location, mat2[] p_value)
        {
            if(m_program == 0U)
                return;

            Bind();
            unsafe
            {
                fixed(float* l_ptr = &p_value[0].m00)
                {
                    Gl.UniformMatrix2(p_location, p_value.Length, false, l_ptr);
                }
            }
        }

        public void SetUniformValue(int p_location, mat3[] p_value)
        {
            if(m_program == 0U)
                return;

            Bind();
            unsafe
            {
                fixed(float* l_ptr = &p_value[0].m00)
                {
                    Gl.UniformMatrix3(p_location, p_value.Length, false, l_ptr);
                }
            }
        }

        public void SetUniformValue(int p_location, mat4[] p_value)
        {
            if(m_program == 0U)
                return;

            Bind();
            unsafe
            {
                fixed(float* l_ptr = &p_value[0].m00)
                {
                    Gl.UniformMatrix4(p_location, p_value.Length, false, l_ptr);
                }
            }
        }
    }
}

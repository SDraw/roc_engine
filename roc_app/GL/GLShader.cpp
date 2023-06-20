#include "stdafx.h"

#include "GLShader.h"

GLuint GLShader::ms_activeProgram = 0U;

GLShader::GLShader()
{
    m_program = 0U;
    m_vertex = 0U;
    m_fragment = 0U;
    m_geometry = 0U;
}

bool GLShader::Create(const char *p_vertex, size_t p_vertexSize, const char *p_fragment, size_t p_fragmentSize, const char *p_geometry, size_t p_geometrySize)
{
    if(m_program == 0U)
    {
        if(p_vertex && (p_vertexSize > 0U))
        {
            m_vertex = glCreateShader(GL_VERTEX_SHADER);
            if(m_vertex != 0U)
            {
                int l_sourceLength = static_cast<int>(p_vertexSize);
                glShaderSource(m_vertex, 1, &p_vertex, &l_sourceLength);
                glCompileShader(m_vertex);

                GLint l_state;
                glGetShaderiv(m_vertex, GL_COMPILE_STATUS, &l_state);
                if(!l_state)
                {
                    GLint l_logSize = 0;
                    std::string l_log;
                    glGetShaderiv(m_vertex, GL_INFO_LOG_LENGTH, &l_logSize);
                    l_log.resize(static_cast<size_t>(l_logSize));
                    glGetShaderInfoLog(m_vertex, l_logSize, &l_logSize, &l_log[0]);
                    l_log.insert(0U, "Vertex shader error: ");
                    m_log.append(l_log);

                    glDeleteShader(m_vertex);
                    m_vertex = 0U;
                }
            }
        }

        if(p_fragment && (p_fragmentSize > 0U))
        {
            m_fragment = glCreateShader(GL_FRAGMENT_SHADER);
            if(m_fragment != 0U)
            {
                int l_sourceLength = static_cast<int>(p_fragmentSize);
                glShaderSource(m_fragment, 1, &p_fragment, &l_sourceLength);
                glCompileShader(m_fragment);

                GLint l_state;
                glGetShaderiv(m_fragment, GL_COMPILE_STATUS, &l_state);
                if(!l_state)
                {
                    GLint l_logSize = 0;
                    std::string l_log;
                    glGetShaderiv(m_fragment, GL_INFO_LOG_LENGTH, &l_logSize);
                    l_log.resize(static_cast<size_t>(l_logSize));
                    glGetShaderInfoLog(m_fragment, l_logSize, &l_logSize, &l_log[0]);
                    l_log.insert(0U, "Fragment shader error: ");
                    m_log.append(l_log);

                    glDeleteShader(m_fragment);
                    m_fragment = 0U;
                }

            }
        }

        if(p_geometry && (p_geometrySize > 0U))
        {
            m_geometry = glCreateShader(GL_GEOMETRY_SHADER);
            if(m_geometry != 0U)
            {
                int l_sourceLength = static_cast<int>(p_geometrySize);
                glShaderSource(m_geometry, 1, &p_geometry, &l_sourceLength);
                glCompileShader(m_geometry);

                GLint l_state;
                glGetShaderiv(m_geometry, GL_COMPILE_STATUS, &l_state);
                if(!l_state)
                {
                    GLint l_logSize = 0;
                    std::string l_log;
                    glGetShaderiv(m_geometry, GL_INFO_LOG_LENGTH, &l_logSize);
                    l_log.resize(static_cast<size_t>(l_logSize));
                    glGetShaderInfoLog(m_geometry, l_logSize, &l_logSize, &l_log[0]);
                    l_log.insert(0U, "Geometry shader error: ");
                    m_log.append(l_log);

                    glDeleteShader(m_geometry);
                    m_geometry = 0U;
                }
            }
        }

        if(m_vertex != 0U || m_fragment != 0U || m_geometry != 0U)
        {
            m_program = glCreateProgram();
            if(m_program != 0U)
            {
                if(m_vertex != 0U) glAttachShader(m_program, m_vertex);
                if(m_fragment != 0U) glAttachShader(m_program, m_fragment);
                if(m_geometry != 0U) glAttachShader(m_program, m_geometry);

                GLint l_link = 0;
                glLinkProgram(m_program);
                glGetProgramiv(m_program, GL_LINK_STATUS, &l_link);
                if(!l_link)
                {
                    GLint l_logLength = 0;
                    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &l_logLength);
                    m_log.resize(static_cast<size_t>(l_logLength));
                    glGetProgramInfoLog(m_program, l_logLength, &l_logLength, &m_log[0]);
                    glDeleteProgram(m_program);
                    m_program = 0U;
                }
                else
                {
                    if(m_vertex != 0U) glDetachShader(m_program, m_vertex);
                    if(m_fragment != 0U) glDetachShader(m_program, m_fragment);
                    if(m_geometry != 0U) glDetachShader(m_program, m_geometry);
                }
            }

            if(m_vertex != 0U)
            {
                glDeleteShader(m_vertex);
                m_vertex = 0U;
            }
            if(m_fragment != 0U)
            {
                glDeleteShader(m_fragment);
                m_fragment = 0U;
            }
            if(m_geometry != 0U)
            {
                glDeleteShader(m_geometry);
                m_geometry = 0U;
            }

        }
    }

    return (m_program != 0U);
}

bool GLShader::Destroy()
{
    if(m_program != 0U)
    {
        if(ms_activeProgram == m_program) ms_activeProgram = 0U;

        glDeleteProgram(m_program);
        m_program = 0U;
    }

    return (m_program == 0U);
}

const std::string& GLShader::GetLog() const
{
    return m_log;
}

GLint GLShader::GetUniformLocation(const char *p_name)
{
    int l_result = -1;
    if(m_program != 0U) l_result = glGetUniformLocation(m_program, p_name);
    return l_result;
}

bool GLShader::GetUniformInfo(GLint p_index, GLenum &p_type, std::string &p_name)
{
    if(m_program != 0U)
    {
        std::vector<GLchar> l_uniformName(256U, '\0');
        GLsizei l_uniformNameSize;
        GLsizei l_uniformSize;
        glGetActiveUniform(m_program, static_cast<GLuint>(p_index), 256, &l_uniformNameSize, &l_uniformSize, &p_type, l_uniformName.data());
        p_name.assign(l_uniformName.data(), l_uniformNameSize);
    }
    return (m_program != 0U);
}

GLint GLShader::GetUniformsCount()
{
    int l_result = 0;
    if(m_program != 0U) glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &l_result);
    return l_result;
}

bool GLShader::SetAttribute(GLuint p_index, const char *p_name)
{
    if(m_program != 0U) glBindAttribLocation(m_program, p_index, p_name);
    return (m_program != 0U);
}

bool GLShader::SetUniform(GLint p_index, const int *p_data, size_t p_count)
{
    if(m_program != 0U)
    {
        if(ms_activeProgram != m_program) glUseProgram(m_program);
        switch(p_count)
        {
            case 1U:
                glUniform1i(p_index, p_data[0U]);
                break;
            case 2U:
                glUniform2i(p_index, p_data[0U], p_data[1U]);
                break;
            case 3U:
                glUniform3i(p_index, p_data[0U], p_data[1U], p_data[2U]);
                break;
            case 4U:
                glUniform4i(p_index, p_data[0U], p_data[1U], p_data[2U], p_data[3U]);
                break;
        }
        if((ms_activeProgram != m_program) && (ms_activeProgram != 0U)) glUseProgram(ms_activeProgram);
    }
    return (m_program != 0U);
}

bool GLShader::SetUniform(GLint p_index, const unsigned int *p_data, size_t p_count)
{
    if(m_program != 0U)
    {
        if(ms_activeProgram != m_program) glUseProgram(m_program);
        switch(p_count)
        {
            case 1U:
                glUniform1ui(p_index, p_data[0U]);
                break;
            case 2U:
                glUniform2ui(p_index, p_data[0U], p_data[1U]);
                break;
            case 3U:
                glUniform3ui(p_index, p_data[0U], p_data[1U], p_data[2U]);
                break;
            case 4U:
                glUniform4ui(p_index, p_data[0U], p_data[1U], p_data[2U], p_data[3U]);
                break;
        }
        if((ms_activeProgram != m_program) && (ms_activeProgram != 0U)) glUseProgram(ms_activeProgram);
    }
    return (m_program != 0U);
}

bool GLShader::SetUniform(GLint p_index, const float *p_data, size_t p_count)
{
    if(m_program != 0U)
    {
        if(ms_activeProgram != m_program) glUseProgram(m_program);
        switch(p_count)
        {
            case 1U:
                glUniform1f(p_index, p_data[0U]);
                break;
            case 2U:
                glUniform2f(p_index, p_data[0U], p_data[1U]);
                break;
            case 3U:
                glUniform3f(p_index, p_data[0U], p_data[1U], p_data[2U]);
                break;
            case 4U:
                glUniform4f(p_index, p_data[0U], p_data[1U], p_data[2U], p_data[3U]);
                break;
        }
        if((ms_activeProgram != m_program) && (ms_activeProgram != 0U)) glUseProgram(ms_activeProgram);
    }
    return (m_program != 0U);
}

bool GLShader::SetUniform(GLint p_index, const double *p_data, size_t p_count)
{
    if(m_program != 0U)
    {
        if(ms_activeProgram != m_program) glUseProgram(m_program);
        switch(p_count)
        {
            case 1U:
                glUniform1d(p_index, p_data[0U]);
                break;
            case 2U:
                glUniform2d(p_index, p_data[0U], p_data[1U]);
                break;
            case 3U:
                glUniform3d(p_index, p_data[0U], p_data[1U], p_data[2U]);
                break;
            case 4U:
                glUniform4d(p_index, p_data[0U], p_data[1U], p_data[2U], p_data[3U]);
                break;
        }
        if((ms_activeProgram != m_program) && (ms_activeProgram != 0U)) glUseProgram(ms_activeProgram);
    }
    return (m_program != 0U);
}

bool GLShader::SetUniform(GLint p_index, size_t p_dim, const float *p_data, size_t p_count)
{
    if(m_program != 0U)
    {
        if(ms_activeProgram != m_program) glUseProgram(m_program);
        switch(p_dim)
        {
            case 2U:
                glUniformMatrix2fv(p_index, static_cast<GLsizei>(p_count), GL_FALSE, p_data);
                break;
            case 3U:
                glUniformMatrix3fv(p_index, static_cast<GLsizei>(p_count), GL_FALSE, p_data);
                break;
            case 4U:
                glUniformMatrix4fv(p_index, static_cast<GLsizei>(p_count), GL_FALSE, p_data);
                break;
        }
        if((ms_activeProgram != m_program) && (ms_activeProgram != 0U)) glUseProgram(ms_activeProgram);
    }
    return (m_program != 0U);
}

bool GLShader::SetUniform(GLint p_index, size_t p_dim, const double *p_data, size_t p_count)
{
    if(m_program != 0U)
    {
        if(ms_activeProgram != m_program) glUseProgram(m_program);
        switch(p_dim)
        {
            case 2U:
                glUniformMatrix2dv(p_index, static_cast<GLsizei>(p_count), GL_FALSE, p_data);
                break;
            case 3U:
                glUniformMatrix3dv(p_index, static_cast<GLsizei>(p_count), GL_FALSE, p_data);
                break;
            case 4U:
                glUniformMatrix4dv(p_index, static_cast<GLsizei>(p_count), GL_FALSE, p_data);
                break;
        }
        if((ms_activeProgram != m_program) && (ms_activeProgram != 0U)) glUseProgram(ms_activeProgram);
    }
    return (m_program != 0U);
}

bool GLShader::Bind()
{
    if((m_program != 0U) && (ms_activeProgram != m_program))
    {
        glUseProgram(m_program);
        ms_activeProgram = m_program;
    }
    return (m_program != 0U);
}

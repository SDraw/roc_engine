#include "stdafx.h"

#include "GL/GLVertexArray.h"

GLuint GLVertexArray::ms_activeName = 0U;

GLVertexArray::GLVertexArray()
{
    m_name = 0U;
}

bool GLVertexArray::Create()
{
    if(m_name == 0U) glGenVertexArrays(1, &m_name);

    return (m_name != 0U);
}

bool GLVertexArray::Destroy()
{
    if(m_name != 0U)
    {
        if(ms_activeName == m_name) ms_activeName = 0U;
        glDeleteVertexArrays(1, &m_name);
        m_name = 0U;
    }
    return (m_name == 0U);
}

bool GLVertexArray::EnableAttribute(GLuint p_index, GLsizei p_size, GLenum p_type)
{
    if(m_name != 0U)
    {
        if(ms_activeName != m_name) glBindVertexArray(m_name);

        glEnableVertexAttribArray(p_index);
        switch(p_type)
        {
            case GL_FLOAT:
                glVertexAttribPointer(p_index, p_size, p_type, GL_FALSE, 0, NULL);
                break;
            case GL_INT:
                glVertexAttribIPointer(p_index, p_size, p_type, 0, NULL);
                break;
        }

        if(ms_activeName != m_name) glBindVertexArray(ms_activeName);
    }

    return (m_name != 0U);
}

bool GLVertexArray::Bind()
{
    if((m_name != 0) && (ms_activeName != m_name))
    {
        glBindVertexArray(m_name);
        ms_activeName = m_name;
    }

    return (m_name != 0U);
}

bool GLVertexArray::Draw(GLenum p_mode, GLsizei p_count)
{
    if(m_name != 0U)
    {
        if(ms_activeName != m_name) glBindVertexArray(m_name);
        glDrawArrays(p_mode, 0, p_count);
        if(ms_activeName != m_name) glBindVertexArray(ms_activeName);
    }
    return (m_name != 0U);
}

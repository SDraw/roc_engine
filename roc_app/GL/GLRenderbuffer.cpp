#include "stdafx.h"

#include "GLRenderbuffer.h"

GLuint GLRenderbuffer::ms_activeName = 0U;

GLRenderbuffer::GLRenderbuffer()
{
    m_name = 0U;
}

bool GLRenderbuffer::Create(GLenum p_format, GLsizei p_width, GLsizei p_height)
{
    if(m_name == 0U)
    {
        glGenRenderbuffers(1, &m_name);
        if(m_name != 0U)
        {
            glBindRenderbuffer(GL_RENDERBUFFER, m_name);
            glRenderbufferStorage(GL_RENDERBUFFER, p_format, p_width, p_height);

            glBindRenderbuffer(GL_RENDERBUFFER, ms_activeName);
        }
    }

    return (m_name != 0U);
}

bool GLRenderbuffer::Destroy()
{
    if(m_name != 0U)
    {
        if(ms_activeName == m_name)
        {
            ms_activeName = 0U;
            glBindRenderbuffer(GL_RENDERBUFFER, ms_activeName);
        }

        glDeleteRenderbuffers(1, &m_name);
        m_name = 0U;
    }

    return (m_name == 0U);
}

GLuint GLRenderbuffer::GetName() const
{
    return m_name;
}

bool GLRenderbuffer::Bind()
{
    if((m_name != 0U) && (ms_activeName != m_name))
    {
        glBindRenderbuffer(GL_RENDERBUFFER, m_name);
        ms_activeName = m_name;
    }

    return (m_name != 0U);
}

void GLRenderbuffer::Reset()
{
    if(ms_activeName != 0U)
    {
        ms_activeName = 0U;
        glBindRenderbuffer(GL_RENDERBUFFER, ms_activeName);
    }
}

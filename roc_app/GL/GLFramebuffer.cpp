#include "stdafx.h"

#include "GLFramebuffer.h"

GLuint GLFramebuffer::ms_activeName = 0U;

GLFramebuffer::GLFramebuffer()
{
    m_name = 0U;
}

bool GLFramebuffer::Create()
{
    if(m_name == 0U) glGenFramebuffers(1, &m_name);
    return (m_name != 0U);
}

bool GLFramebuffer::Destroy()
{
    if(m_name != 0U)
    {
        if(ms_activeName == m_name)
        {
            ms_activeName = 0U;
            glBindFramebuffer(GL_FRAMEBUFFER, ms_activeName);
        }

        glDeleteFramebuffers(1, &m_name);
        m_name = 0U;
    }
    return (m_name == 0U);
}

bool GLFramebuffer::SetRenderbuffer(GLenum p_attachment, GLuint p_name)
{
    if(m_name != 0U)
    {
        if(ms_activeName != m_name) glBindFramebuffer(GL_FRAMEBUFFER, m_name);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, p_attachment, GL_RENDERBUFFER, p_name);
        if(ms_activeName != m_name) glBindFramebuffer(GL_FRAMEBUFFER, ms_activeName);
    }

    return (m_name != 0U);
}

bool GLFramebuffer::SetTexture2D(GLenum p_attachment, GLuint p_name)
{
    if(m_name != 0U)
    {
        if(ms_activeName != m_name) glBindFramebuffer(GL_FRAMEBUFFER, m_name);
        glFramebufferTexture2D(GL_FRAMEBUFFER, p_attachment, GL_TEXTURE_2D, p_name, 0);
        if(ms_activeName != m_name) glBindFramebuffer(GL_FRAMEBUFFER, ms_activeName);
    }

    return (m_name != 0U);
}

bool GLFramebuffer::SetDrawBuffer(GLenum p_buffer)
{
    if(m_name != 0U)
    {
        if(ms_activeName != m_name) glBindFramebuffer(GL_FRAMEBUFFER, m_name);
        glDrawBuffer(p_buffer);
        if(ms_activeName != m_name) glBindFramebuffer(GL_FRAMEBUFFER, ms_activeName);
    }

    return (m_name != 0U);
}

bool GLFramebuffer::SetReadBuffer(GLenum p_buffer)
{
    if(m_name != 0U)
    {
        if(ms_activeName != m_name) glBindFramebuffer(GL_FRAMEBUFFER, m_name);
        glReadBuffer(p_buffer);
        if(ms_activeName != m_name) glBindFramebuffer(GL_FRAMEBUFFER, ms_activeName);
    }

    return (m_name != 0U);
}

bool GLFramebuffer::Bind()
{
    if((m_name != 0U) && (ms_activeName != m_name))
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_name);
        ms_activeName = m_name;
    }
    return (m_name != 0U);
}

void GLFramebuffer::Reset()
{
    if(ms_activeName != 0U)
    {
        ms_activeName = 0U;
        glBindFramebuffer(GL_FRAMEBUFFER, ms_activeName);
    }
}

GLenum GLFramebuffer::CheckStatus()
{
    return glCheckFramebufferStatus(GL_FRAMEBUFFER);
}

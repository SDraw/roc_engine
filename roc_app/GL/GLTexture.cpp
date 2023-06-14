#include "stdafx.h"

#include "GL/GLTexture.h"

GLuint GLTexture::ms_activeSlot = 0U;
std::array<GLuint, 16U> GLTexture::ms_activeName = { 0U };
std::array<GLenum, 16U> GLTexture::ms_activeNameType = { GL_NONE };

GLTexture::GLTexture()
{
    m_name = 0U;
    m_internalType = GL_NONE;
}

GLuint GLTexture::GetName() const
{
    return m_name;
}

bool GLTexture::Destroy()
{
    if(m_name != 0U)
    {
        for(size_t i = 0U; i < 16U; i++)
        {
            if(ms_activeName[i] == m_name)
            {
                ms_activeName[i] = 0U;
                ms_activeNameType[i] = GL_NONE;
            }
        }

        glDeleteTextures(1, &m_name);
        m_name = 0U;
    }
    return (m_name == 0U);
}

bool GLTexture::Bind(GLenum p_slot)
{
    if((m_name != 0U) && (ms_activeName[p_slot] != m_name))
    {
        if(ms_activeSlot != p_slot)
        {
            glActiveTexture(GL_TEXTURE0 + p_slot);
            ms_activeSlot = p_slot;
        }

        ms_activeName[ms_activeSlot] = m_name;
        ms_activeNameType[ms_activeSlot] = m_internalType;
        glBindTexture(m_internalType, m_name);
    }

    return (m_name != 0U);
}

void GLTexture::Rebind()
{
    for(size_t i = 0U; i < 16U; i++)
    {
        if(ms_activeName[i] != 0U)
        {
            glActiveTexture(GL_TEXTURE0 + static_cast<unsigned int>(i));
            glBindTexture(ms_activeNameType[i], ms_activeName[i]);
        }
    }

    glActiveTexture(GL_TEXTURE0 + ms_activeSlot);
}

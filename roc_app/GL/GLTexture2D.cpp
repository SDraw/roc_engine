#include "stdafx.h"

#include "GL/GLTexture2D.h"

GLTexture2D::GLTexture2D()
{
    m_internalType = GL_TEXTURE_2D;
}

bool GLTexture2D::Create(GLsizei p_width, GLsizei p_height, GLint p_format, GLenum p_dataFormat, const void *p_data, GLenum p_filter)
{
    if(m_name == 0U)
    {
        glGenTextures(1, &m_name);
        if(m_name != 0U)
        {
            glBindTexture(GL_TEXTURE_2D, m_name);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, p_filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, p_filter);
            glTexImage2D(GL_TEXTURE_2D, 0, p_format, p_width, p_height, 0, p_dataFormat, GL_UNSIGNED_BYTE, p_data);

            if(ms_activeName[ms_activeSlot] != 0U) glBindTexture(ms_activeNameType[ms_activeSlot], ms_activeName[ms_activeSlot]);
        }
    }
    return (m_name != 0U);
}

bool GLTexture2D::SetSwizzle(GLenum p_swizzle, const GLint *p_order)
{
    if(m_name != 0U)
    {
        if(ms_activeName[ms_activeSlot] != m_name) glBindTexture(GL_TEXTURE_2D, m_name);
        glTexParameteriv(GL_TEXTURE_2D, p_swizzle, p_order);
        if((ms_activeName[ms_activeSlot] != m_name) && (ms_activeName[ms_activeSlot] != 0U)) glBindTexture(ms_activeNameType[ms_activeSlot], ms_activeName[ms_activeSlot]);
    }
    return (m_name != 0U);
}

bool GLTexture2D::SetCompareFunc(GLenum p_func)
{
    if(m_name != 0U)
    {
        if(ms_activeName[ms_activeSlot] != m_name) glBindTexture(GL_TEXTURE_2D, m_name);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, p_func);
        if((ms_activeName[ms_activeSlot] != m_name) && (ms_activeName[ms_activeSlot] != 0U)) glBindTexture(ms_activeNameType[ms_activeSlot], ms_activeName[ms_activeSlot]);
    }
    return (m_name != 0U);
}

bool GLTexture2D::SetCompareMode(GLenum p_mode)
{
    if(m_name != 0U)
    {
        if(ms_activeName[ms_activeSlot] != m_name) glBindTexture(GL_TEXTURE_2D, m_name);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, p_mode);
        if((ms_activeName[ms_activeSlot] != m_name) && (ms_activeName[ms_activeSlot] != 0U)) glBindTexture(ms_activeNameType[ms_activeSlot], ms_activeName[ms_activeSlot]);
    }
    return (m_name != 0U);
}

bool GLTexture2D::SetWrap(GLenum p_wrap)
{
    if(m_name != 0U)
    {
        if(ms_activeName[ms_activeSlot] != m_name) glBindTexture(GL_TEXTURE_2D, m_name);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, p_wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, p_wrap);
        if((ms_activeName[ms_activeSlot] != m_name) && (ms_activeName[ms_activeSlot] != 0U)) glBindTexture(ms_activeNameType[ms_activeSlot], ms_activeName[ms_activeSlot]);
    }
    return (m_name != 0U);
}

bool GLTexture2D::Update(GLint p_posX, GLint p_posY, GLsizei p_width, GLsizei p_height, GLenum p_dataFormat, const void *p_data)
{
    if(m_name != 0U)
    {
        if(ms_activeName[ms_activeSlot] != m_name) glBindTexture(GL_TEXTURE_2D, m_name);
        glTexSubImage2D(GL_TEXTURE_2D, 0, p_posX, p_posY, p_width, p_height, p_dataFormat, GL_UNSIGNED_BYTE, p_data);
        if((ms_activeName[ms_activeSlot] != m_name) && (ms_activeName[ms_activeSlot] != 0U)) glBindTexture(ms_activeNameType[ms_activeSlot], ms_activeName[ms_activeSlot]);
    }

    return (m_name != 0U);
}

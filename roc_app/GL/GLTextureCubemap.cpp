#include "stdafx.h"

#include "GL/GLTextureCubemap.h"

GLTextureCubemap::GLTextureCubemap()
{
    m_internalType = GL_TEXTURE_CUBE_MAP;
}

bool GLTextureCubemap::Create(GLsizei p_width, GLsizei p_height, GLint p_format, GLenum p_dataFormat, const std::vector<const void*> &p_data, GLenum p_filter)
{
    if((m_name == 0U) && (p_data.size() >= 6U))
    {
        glGenTextures(1, &m_name);
        if(m_name != 0U)
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_name);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, p_filter);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, p_filter);

            for(size_t i = 0U; i < 6U; i++) glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<GLenum>(i), 0, p_format, p_width, p_height, 0, p_dataFormat, GL_UNSIGNED_BYTE, p_data[i]);

            if(ms_activeName[ms_activeSlot] != 0U) glBindTexture(ms_activeNameType[ms_activeSlot], ms_activeName[ms_activeSlot]);
        }
    }
    return (m_name != 0U);
}

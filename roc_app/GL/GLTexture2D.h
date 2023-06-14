#pragma once
#include "GL/GLTexture.h"

class GLTexture2D final : public GLTexture
{
    GLTexture2D(const GLTexture2D &that) = delete;
    GLTexture2D& operator=(const GLTexture2D &that) = delete;
public:
    GLTexture2D();
    ~GLTexture2D() = default;

    bool Create(GLsizei p_width, GLsizei p_height, GLint p_format, GLenum p_dataFormat, const void *p_data, GLenum p_filter = GL_NEAREST);

    bool SetSwizzle(GLenum p_swizzle, const GLint *p_order);
    bool SetCompareFunc(GLenum p_func);
    bool SetCompareMode(GLenum p_mode);
    bool SetWrap(GLenum p_wrap);
    bool Update(GLint p_posX, GLint p_posY, GLsizei p_width, GLsizei p_height, GLenum p_dataFormat, const void *p_data);

};

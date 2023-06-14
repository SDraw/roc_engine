#pragma once
#include "GL/GLTexture.h"

class GLTextureCubemap final : public GLTexture
{
    GLTextureCubemap(const GLTextureCubemap &that) = delete;
    GLTextureCubemap& operator=(const GLTextureCubemap &that) = delete;
public:
    GLTextureCubemap();
    ~GLTextureCubemap() = default;

    bool Create(GLsizei p_width, GLsizei p_height, GLint p_format, GLenum p_dataFormat, const std::vector<const void*> &p_data, GLenum p_filter = GL_NEAREST);
};

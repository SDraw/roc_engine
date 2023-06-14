#pragma once

class GLFramebuffer final
{
    GLuint m_name;

    __declspec(thread) static GLuint ms_activeName;

    GLFramebuffer(const GLFramebuffer &that) = delete;
    GLFramebuffer& operator=(const GLFramebuffer &that) = delete;
public:
    GLFramebuffer();
    ~GLFramebuffer() = default;

    bool Create();
    bool Destroy();

    bool SetRenderbuffer(GLenum p_attachment, GLuint p_name);
    bool SetTexture2D(GLenum p_attachment, GLuint p_name);
    bool SetDrawBuffer(GLenum p_buffer);
    bool SetReadBuffer(GLenum p_buffer);

    bool Bind();
    static void Reset();

    static GLenum CheckStatus();
};


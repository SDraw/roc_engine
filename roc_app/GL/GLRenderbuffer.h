#pragma once

class GLRenderbuffer final
{
    GLuint m_name;

    __declspec(thread) static GLuint ms_activeName;

    GLRenderbuffer(const GLRenderbuffer &that) = delete;
    GLRenderbuffer& operator=(const GLRenderbuffer &that) = delete;
public:
    GLRenderbuffer();
    ~GLRenderbuffer() = default;

    bool Create(GLenum p_format, GLsizei p_width, GLsizei p_height);
    bool Destroy();

    GLuint GetName() const;
    
    bool Bind();
    static void Reset();
};

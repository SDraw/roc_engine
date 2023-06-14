#pragma once

class GLVertexArray final
{
    GLuint m_name;

    __declspec(thread) static GLuint ms_activeName;

    GLVertexArray(const GLVertexArray &that) = delete;
    GLVertexArray& operator=(const GLVertexArray &that) = delete;
public:
    GLVertexArray();
    ~GLVertexArray() = default;

    bool Create();
    bool Destroy();

    bool EnableAttribute(GLuint p_index, GLsizei p_size, GLenum p_type);

    bool Bind();
    bool Draw(GLenum p_mode, GLsizei p_count);
};

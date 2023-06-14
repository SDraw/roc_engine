#pragma once

class GLArrayBuffer final
{
    GLuint m_name;

    __declspec(thread) static GLuint ms_activeName;

    GLArrayBuffer(const GLArrayBuffer &that) = delete;
    GLArrayBuffer& operator=(const GLArrayBuffer &that) = delete;
public:
    GLArrayBuffer();
    ~GLArrayBuffer() = default;

    bool Create(size_t p_size, const void *p_data, GLenum p_usage);
    bool Destroy();

    bool Bind();
    bool Update(GLintptr p_offset, GLsizeiptr p_size, const void *p_data);
};

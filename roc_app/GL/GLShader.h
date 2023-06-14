#pragma once

class GLShader final
{
    GLuint m_program;
    GLuint m_vertex;
    GLuint m_fragment;
    GLuint m_geometry;

    std::string m_log;

    __declspec(thread) static GLuint ms_activeProgram;

    GLShader(const GLShader &that) = delete;
    GLShader& operator=(const GLShader &that) = delete;
public:
    GLShader();
    ~GLShader() = default;

    bool Create(const char *p_vertex, size_t p_vertexSize, const char *p_fragment, size_t p_fragmentSize, const char *p_geometry, size_t p_geometrySize);
    bool Destroy();

    const std::string& GetLog() const;
    GLint GetUniformLocation(const char *p_name);
    bool GetUniformInfo(GLint p_index, GLenum &p_type, std::string &p_name);
    GLint GetUniformsCount();

    bool SetAttribute(GLuint p_index, const char *p_name);
    bool SetUniform(GLint p_index, const int *p_data, size_t p_count);
    bool SetUniform(GLint p_index, const unsigned int *p_data, size_t p_count);
    bool SetUniform(GLint p_index, const float *p_data, size_t p_count);
    bool SetUniform(GLint p_index, const double *p_data, size_t p_count);
    bool SetUniform(GLint p_index, size_t p_dim, const float *p_data, size_t p_count);
    bool SetUniform(GLint p_index, size_t p_dim, const double *p_data, size_t p_count);

    bool Bind();
};

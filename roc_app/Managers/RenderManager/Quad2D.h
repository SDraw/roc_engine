#pragma once

class GLArrayBuffer;
class GLVertexArray;

namespace ROC
{

class Quad2D final
{
    std::array<glm::vec3, 6U> m_vertex;
    glm::vec2 m_size;

    enum QuadBufferIndex : size_t
    {
        QBI_Vertex = 0U,
        QBI_UV,

        QBI_BufferCount
    };
    enum QuadBufferAttribute : size_t
    {
        QBA_Vertex = 0U,
        QBA_UV = 2U
    };
    std::array<GLArrayBuffer*, QBI_BufferCount> m_arrayBuffers;
    GLVertexArray *m_vertexArray;

    Quad2D(const Quad2D &that) = delete;
    Quad2D& operator=(const Quad2D &that) = delete;
public:
    Quad2D();
    ~Quad2D();

    void SetTransformation(const glm::vec2 &p_size);
    void Draw();
};

}

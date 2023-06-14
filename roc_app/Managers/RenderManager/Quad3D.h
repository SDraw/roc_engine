#pragma once

class GLArrayBuffer;
class GLVertexArray;

namespace ROC
{

class Quad3D final
{
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec2 m_size;
    glm::mat4 m_matrix;

    enum QuadBufferIndex : size_t
    {
        QBI_Vertex = 0U,
        QBI_Normal,
        QBI_UV,

        QBI_BufferCount
    };
    std::array<glm::vec3, 6U> m_vertex;
    std::array<GLArrayBuffer*, QBI_BufferCount> m_arrayBuffers;
    GLVertexArray *m_vertexArray;

    Quad3D(const Quad3D &that) = delete;
    Quad3D& operator=(const Quad3D &that) = delete;
public:
    Quad3D();
    ~Quad3D();

    const glm::mat4& GetMatrix() const;

    void SetTransformation(const glm::vec3 &p_pos, const glm::quat &p_rot, const glm::vec2 &p_size);
    void Draw();
};

}

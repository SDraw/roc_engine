#include "stdafx.h"

#include "Managers/RenderManager/Quad3D.h"
#include "GL/GLArrayBuffer.h"
#include "GL/GLVertexArray.h"

namespace ROC
{

extern const glm::mat4 g_identityMatrix;
extern const glm::vec2 g_emptyVec2;
extern const glm::vec3 g_emptyVec3;
extern const glm::quat g_defaultRotation;

extern const std::array<float, 12U> g_quadVertexUV;
extern const std::array<float, 18U> g_quadVertexNormals;
const size_t g_quad3DVerticesCount = 6U;

}

ROC::Quad3D::Quad3D()
{
    m_position = g_emptyVec3;
    m_rotation = g_defaultRotation;
    m_size = g_emptyVec2;
    m_matrix = g_identityMatrix;

    m_vertexArray = new GLVertexArray();
    m_vertexArray->Create();
    m_vertexArray->Bind();

    for(size_t i = 0U; i < QBI_BufferCount; i++) m_arrayBuffers[i] = new GLArrayBuffer();

    m_arrayBuffers[QBI_Vertex]->Create(sizeof(glm::vec3) * g_quad3DVerticesCount, NULL, GL_DYNAMIC_DRAW);
    m_arrayBuffers[QBI_Vertex]->Bind();
    m_vertexArray->EnableAttribute(QBI_Vertex, 3, GL_FLOAT);

    m_arrayBuffers[QBI_Normal]->Create(sizeof(glm::vec3) * g_quad3DVerticesCount, g_quadVertexNormals.data(), GL_STATIC_DRAW);
    m_arrayBuffers[QBI_Normal]->Bind();
    m_vertexArray->EnableAttribute(QBI_Normal, 3, GL_FLOAT);

    m_arrayBuffers[QBI_UV]->Create(sizeof(glm::vec2) * g_quad3DVerticesCount, g_quadVertexUV.data(), GL_STATIC_DRAW);
    m_arrayBuffers[QBI_UV]->Bind();
    m_vertexArray->EnableAttribute(QBI_UV, 2, GL_FLOAT);

    for(auto &l_vertex : m_vertex) l_vertex.z = 0.f;
}

ROC::Quad3D::~Quad3D()
{
    for(size_t i = 0U; i < QBI_BufferCount; i++)
    {
        m_arrayBuffers[i]->Destroy();
        delete m_arrayBuffers[i];
    }

    m_vertexArray->Destroy();
    delete m_vertexArray;
}

void ROC::Quad3D::SetTransformation(const glm::vec3 &p_pos, const glm::quat &p_rot, const glm::vec2 &p_size)
{
    if(p_pos != m_position || p_rot != m_rotation)
    {
        std::memcpy(&m_position, &p_pos, sizeof(glm::vec3));
        std::memcpy(&m_rotation, &p_rot, sizeof(glm::quat));

        btTransform l_transform = btTransform::getIdentity();
        btVector3 l_position(m_position.x, m_position.y, m_position.z);
        btQuaternion l_rotation(m_rotation.x, m_rotation.y, m_rotation.z, m_rotation.w);
        l_transform.setOrigin(l_position);
        l_transform.setRotation(l_rotation);
        l_transform.getOpenGLMatrix(glm::value_ptr(m_matrix));
    }
    if(p_size != m_size)
    {
        std::memcpy(&m_size, &p_size, sizeof(glm::vec2));
        m_vertex[0].x = m_vertex[1].x = m_vertex[3].x = -m_size.x / 2.f;
        m_vertex[0].y = m_vertex[3].y = m_vertex[5].y = m_size.y / 2.f;
        m_vertex[2].x = m_vertex[4].x = m_vertex[5].x = m_size.x / 2.f;
        m_vertex[1].y = m_vertex[2].y = m_vertex[4].y = -m_size.y / 2.f;

        m_arrayBuffers[QBI_Vertex]->Bind();
        m_arrayBuffers[QBI_Vertex]->Update(0, sizeof(glm::vec3)*g_quad3DVerticesCount, m_vertex.data());
    }
}

const glm::mat4& ROC::Quad3D::GetMatrix() const
{
    return m_matrix;
}

void ROC::Quad3D::Draw()
{
    m_vertexArray->Bind();
    m_vertexArray->Draw(GL_TRIANGLES, g_quad3DVerticesCount);
}

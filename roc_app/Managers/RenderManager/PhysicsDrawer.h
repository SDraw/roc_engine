#pragma once

class GLArrayBuffer;
class GLVertexArray;

namespace ROC
{

class PhysicsDrawer final : public btIDebugDraw
{
    enum PhysicsDrawerBufferIndex : size_t
    {
        PDBI_Vertex = 0U,
        PDBI_Color,

        PDBI_BufferCount
    };
    std::array<GLArrayBuffer*, PDBI_BufferCount> m_arrayBuffers;
    GLVertexArray *m_vertexArray;

    std::vector<glm::vec3> m_lines;
    std::vector<glm::vec3> m_colors;

    PhysicsDrawer(const PhysicsDrawer &that) = delete;
    PhysicsDrawer& operator=(const PhysicsDrawer &that) = delete;

    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
    void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
    void reportErrorWarning(const char* warningString);
    void draw3dText(const btVector3& location, const char* textString);
    void setDebugMode(int debugMode);
    int	getDebugMode() const;
public:
    PhysicsDrawer();
    ~PhysicsDrawer();

    void Draw(float p_width);
};

}

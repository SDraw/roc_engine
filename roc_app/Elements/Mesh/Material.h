#pragma once

class GLArrayBuffer;
class GLVertexArray;

namespace ROC
{

class Texture;

class Material final
{
    int m_verticesCount;
    enum MaterialBufferIndex : size_t
    {
        MBI_Vertex = 0U,
        MBI_Normal,
        MBI_UV,
        MBI_Weight,
        MBI_WeightIndex,

        MBI_BufferCount
    };
    std::array<GLArrayBuffer*, MBI_BufferCount> m_arrayBuffers;
    GLVertexArray *m_vertexArray;

    unsigned char m_type;
    glm::vec4 m_params;
    Texture *m_texture;

    static Texture *ms_dummyTexture;

    Material(const Material &that) = delete;
    Material& operator=(const Material &that) = delete;
public:
    enum MaterialPropertyBit : unsigned char
    {
        MPB_Shading = (1U << 0),
        MPB_Depth = (1U << 1),
        MPB_Transparency = (1U << 2),
        MPB_Doubleside = (1U << 3),
        MPB_Filtering = (1U << 4),
        MPB_Compression = (1U << 5)
    };

    Material();
    ~Material();

    unsigned char GetType() const;
    const glm::vec4& GetParams() const;

    bool IsDoubleSided() const;
    bool IsTransparent() const;
    bool IsShady() const;
    bool HasDepth() const;
    bool IsCompressed() const;
    unsigned char GetFilteringType() const;

    void LoadVertices(const std::vector<glm::vec3> &p_vector);
    void LoadUVs(const std::vector<glm::vec2> &p_vector);
    void LoadNormals(const std::vector<glm::vec3> &p_vector);
    void LoadWeights(const std::vector<glm::vec4> &p_vector);
    void LoadIndices(const std::vector<glm::ivec4> &p_vector);
    void LoadTexture(const std::string &p_path);
    void Generate();

    void SetType(unsigned char p_type);

    void SetParams(const glm::vec4 &p_params);

    Texture* GetTexture() const;

    void Draw(bool p_textured = true);

    static void InitStaticResources();
    static void ReleaseStaticResources();
};

}

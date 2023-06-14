#pragma once
#include "Interfaces/IFont.h"
#include "Elements/Element.h"

class GLTexture2D;
class GLArrayBuffer;
class GLVertexArray;

namespace ROC
{

class Font final : public Element, public IFont
{
    static FT_Library ms_library;
    FT_Face m_face;
    float m_size;

    GLTexture2D *m_atlasTexture;
    rbp::MaxRectsBinPack *m_atlasPack;
    glm::vec2 m_atlasOffset;
    glm::ivec2 m_atlasSize;

    struct FontCharacterData
    {
        glm::vec4 m_atlasPosition;
        glm::ivec2 m_size;
        glm::ivec2 m_bearing;
        float m_advance;
    };
    std::unordered_map<unsigned int, FontCharacterData*> m_charMap;
    std::unordered_map<unsigned int, FontCharacterData*>::iterator m_charIter;
    std::unordered_map<unsigned int, FontCharacterData*>::iterator m_charMapEnd;

    enum FontBufferIndex : size_t
    {
        FBI_Vertex = 0U,
        FBI_UV,

        FBI_BufferCount
    };
    enum FontBufferAttribute : size_t
    {
        FBA_Vertex = 0U,
        FBA_UV = 2U
    };
    static std::array<GLArrayBuffer*, FBI_BufferCount> ms_arrayBuffers;
    static GLVertexArray *ms_vertexArray;

    static std::vector<glm::vec3> ms_vertices;
    static std::vector<glm::vec2> ms_uv;

    unsigned char m_filteringType;

    bool m_loaded;

    Font(const Font &that) = delete;
    Font& operator=(const Font &that) = delete;

    bool LoadChar(unsigned int p_char);
public:
    Font();
    ~Font();

    bool Load(const std::string &p_path, int p_size, const glm::ivec2 &p_atlas, unsigned char p_filter);

    unsigned char GetFiltering() const;
    float GetGlyphSize() const;

    void Draw(const sf::String &p_text, const glm::vec2 &p_pos);

    static void InitStaticResources();
    static void ReleaseStaticResources();
};

}

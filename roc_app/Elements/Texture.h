#pragma once
#include "Interfaces/ITexture.h"
#include "Elements/Drawable.h"
#include "Elements/Element.h"

class GLTexture;

namespace ROC
{

class Texture final : public Element, public Drawable, public ITexture
{
    unsigned char m_type;
    glm::ivec2 m_size;
    bool m_compressed;

    GLTexture *m_texture;

    Texture(const Texture &that) = delete;
    Texture& operator=(const Texture &that) = delete;
public:
    Texture();
    ~Texture();

    bool Load(const std::string &p_path, unsigned char p_type, unsigned char p_filter, bool p_compress);
    bool LoadCubemap(const std::vector<std::string> &p_path, unsigned char p_filter, bool p_compress);
    bool LoadDummy();

    bool IsTransparent() const;
    bool IsCubic() const;
    bool IsCompressed() const;

    const glm::ivec2& GetSize() const;

    void Bind(size_t p_slot = 0U);
};

}

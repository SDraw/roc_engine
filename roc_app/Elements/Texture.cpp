#include "stdafx.h"

#include "Elements/Texture.h"
#include "GL/GLTexture2D.h"
#include "GL/GLTextureCubemap.h"

namespace ROC
{

const std::array<unsigned char, 12U> g_textureDummyPattern =
{
    0x7FU, 0x7FU, 0x7FU,
    0xF7U, 0x94U, 0x1DU,
    0xF7U, 0x94U, 0x1DU,
    0x7FU, 0x7FU, 0x7FU
};
const glm::ivec2 g_textureDummySize(2, 2);

}

ROC::Texture::Texture()
{
    m_elementType = ET_Texture;

    m_type = TT_None;
    m_filtering = DFT_None;
    m_compressed = false;
    m_texture = nullptr;
}

ROC::Texture::~Texture()
{
    if(m_texture)
    {
        m_texture->Destroy();
        delete m_texture;
        m_texture = nullptr;
    }
}

bool ROC::Texture::Load(const std::string &p_path, unsigned char p_type, unsigned char p_filter, bool p_compress)
{
    if(!m_texture)
    {
        sf::Image l_image;
        if(l_image.loadFromFile(p_path))
        {
            sf::Vector2u l_imageSize = l_image.getSize();
            m_size.x = static_cast<int>(l_imageSize.x);
            m_size.y = static_cast<int>(l_imageSize.y);

            m_type = glm::clamp<unsigned char>(p_type, TT_RGB, TT_RGBA);
            m_filtering = glm::clamp<unsigned char>(p_filter, DFT_Nearest, DFT_Linear);
            m_compressed = p_compress;

            m_texture = new GLTexture2D();
            dynamic_cast<GLTexture2D*>(m_texture)->Create(m_size.x, m_size.y, (p_type == TT_RGB) ? (m_compressed ? GL_COMPRESSED_RGB : GL_RGB8) : (m_compressed ? GL_COMPRESSED_RGBA : GL_RGBA8), GL_RGBA, l_image.getPixelsPtr(), GL_NEAREST + m_filtering);
        }
    }
    return (m_texture != nullptr);
}

bool ROC::Texture::LoadCubemap(const std::vector<std::string> &p_path, unsigned char p_filter, bool p_compress)
{
    if(!m_texture && (p_path.size() >= 6U))
    {
        m_type = TT_Cubemap;
        m_filtering = glm::clamp<unsigned char>(m_filtering, DFT_Nearest, DFT_Linear);
        m_compressed = p_compress;

        std::array<sf::Image, 6U> l_images;
        for(size_t i = 0U; i < 6U; i++) l_images[i].loadFromFile(p_path[i]);

        sf::Vector2u l_size = l_images[0U].getSize();
        bool l_valid = true;

        for(size_t i = 1U; i < 6U; i++)
        {
            if(l_size != l_images[i].getSize())
            {
                l_valid = false;
                break;
            }
        }

        if(l_valid)
        {
            std::vector<const void*> l_data;
            for(size_t i = 0U; i < 6U; i++) l_data.push_back(l_images[i].getPixelsPtr());

            m_texture = new GLTextureCubemap();
            dynamic_cast<GLTextureCubemap*>(m_texture)->Create(l_size.x, l_size.y, m_compressed ? GL_COMPRESSED_RGB : GL_RGB8, GL_RGBA, l_data, GL_NEAREST + m_filtering);
        }
    }
    return (m_texture != nullptr);
}

bool ROC::Texture::LoadDummy()
{
    if(!m_texture)
    {
        std::memcpy(&m_size, &g_textureDummySize, sizeof(glm::ivec2));
        m_filtering = DFT_Nearest;
        m_type = TT_RGB;
        m_compressed = false;

        m_texture = new GLTexture2D();
        dynamic_cast<GLTexture2D*>(m_texture)->Create(m_size.x, m_size.y, GL_COMPRESSED_RGB, GL_RGB, g_textureDummyPattern.data(), GL_NEAREST);
    }
    return (m_texture != nullptr);
}

const glm::ivec2&ROC::Texture::GetSize() const
{
    return m_size;
}

bool ROC::Texture::IsTransparent() const
{
    return (m_type == TT_RGBA);
}

bool ROC::Texture::IsCubic() const
{
    return (m_type == TT_Cubemap);
}

bool ROC::Texture::IsCompressed() const
{
    return m_compressed;
}

void ROC::Texture::Bind(size_t p_slot)
{
    if(m_texture) m_texture->Bind(static_cast<GLenum>(p_slot));
}

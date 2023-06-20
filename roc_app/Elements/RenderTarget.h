#pragma once
#include "Interfaces/IRenderTarget.h"
#include "Elements/Drawable.h"
#include "Elements/Element.h"

class GLTexture2D;
class GLFramebuffer;
class GLRenderbuffer;

namespace ROC
{

class RenderTarget final : public Element, public Drawable, public IRenderTarget
{
    unsigned char m_type;

    GLFramebuffer *m_framebuffer;
    GLRenderbuffer *m_renderbuffer;
    GLTexture2D *m_texture;
    glm::ivec2 m_size;

    glm::vec4 m_clearColor;
    glm::bvec2 m_clearBuffer; // Depth, color

    RenderTarget(const RenderTarget &that) = delete;
    RenderTarget& operator=(const RenderTarget &that) = delete;

    void Clear();
public:
    RenderTarget();
    ~RenderTarget();

    bool Create(unsigned char p_type, const glm::ivec2 &p_size, unsigned char p_filter);

    const glm::ivec2& GetSize() const;

    bool IsTransparent() const;
    bool IsCubic() const;
    bool IsShadowType() const;

    bool SetProperty(RenderTargetProperty p_prop, const void *p_val);

    GLTexture2D* GetGLTexture() const;

    void Bind(size_t p_slot);
    void BindBuffer(bool p_clear = true);
    static void ResetBuffer();
};

}

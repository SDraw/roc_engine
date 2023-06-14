#pragma once
#include "Interfaces/IDrawable.h"

namespace ROC
{

class Drawable : public IDrawable
{
    Drawable(const Drawable &that) = delete;
    Drawable& operator=(const Drawable &that) = delete;
public:
    virtual bool IsTransparent() const = 0;
    virtual bool IsCubic() const = 0;

    virtual const glm::ivec2& GetSize() const = 0;
    unsigned char GetFiltering() const;

    virtual void Bind(size_t p_slot = 0U) = 0;
protected:
    unsigned char m_filtering;

    Drawable();
    virtual ~Drawable() = default;
};

}

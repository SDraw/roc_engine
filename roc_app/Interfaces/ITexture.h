#pragma once

namespace ROC
{

class ITexture
{
protected:
    ~ITexture() = default;
public:
    enum TextureType : unsigned char
    {
        TT_RGB,
        TT_RGBA,
        TT_Cubemap,

        TT_None = 0xFFU
    };

    virtual bool IsCompressed() const = 0;
};

}

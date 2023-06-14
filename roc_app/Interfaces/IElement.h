#pragma once

namespace ROC
{

class IElement
{
public:
    enum ElementType : unsigned char
    {
        ET_Model = 0U,
        ET_Mesh,
        ET_Animation,
        ET_Scene,
        ET_Camera,
        ET_Light,
        ET_RenderTarget,
        ET_Shader,
        ET_Sound,
        ET_Texture,
        ET_Font,
        ET_Collision,

        ET_Invalid = 0xFFU
    };

    virtual unsigned char GetElementType() const = 0;
    virtual const char* GetElementTypeName() const = 0;
};

}

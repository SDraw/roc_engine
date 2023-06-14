#pragma once
#include "IElement.h"

namespace ROC
{

class IDrawable;

class IShader
{
public:
    enum ShaderUniformType : unsigned char
    {
        SUT_Float = 0U,
        SUT_Float2,
        SUT_Float3,
        SUT_Float4,

        SUT_Int,
        SUT_Int2,
        SUT_Int3,
        SUT_Int4,

        SUT_UInt,
        SUT_UInt2,
        SUT_UInt3,
        SUT_UInt4,

        SUT_Double,
        SUT_Double2,
        SUT_Double3,
        SUT_Double4,

        SUT_FloatMat2,
        SUT_FloatMat3,
        SUT_FloatMat4,

        SUT_DoubleMat2,
        SUT_DoubleMat3,
        SUT_DoubleMat4,
    };

    virtual bool Attach(IDrawable *p_drawable, const char *p_uniform) = 0;
    virtual bool Detach(IDrawable *p_drawable) = 0;
    virtual bool HasAttached(IDrawable *p_drawable) const = 0;
    virtual bool SetUniformValue(const char *p_name, ShaderUniformType p_type, const void *p_data, size_t p_size) = 0;
};

}

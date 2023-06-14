#pragma once

namespace ROC
{

class ShaderUniform final
{
    GLint m_uniformName;
    unsigned char m_type;
    unsigned char *m_data;
    size_t m_dataSize;
    size_t m_count;
    bool m_updated;
public:
    ShaderUniform(GLint p_uniform, unsigned char p_type, size_t p_dataSize, size_t p_count = 1U);
    ~ShaderUniform();

    void SetData(const void *p_data, size_t p_dataSize);
    const void* GetData() const;

    GLint GetUniformName() const;
    unsigned char GetType() const;
    size_t GetCount() const;

    bool IsUpdated() const;
    void ResetUpdate();
};

}

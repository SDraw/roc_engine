#include "stdafx.h"

#include "Elements/Shader/ShaderUniform.h"

ROC::ShaderUniform::ShaderUniform(GLint p_uniform, unsigned char p_type, size_t p_dataSize, size_t p_count)
{
    m_uniformName = p_uniform;
    m_type = p_type;
    m_data = ((p_dataSize > 0U) ? new unsigned char[p_dataSize] : nullptr);
    m_dataSize = p_dataSize;
    m_count = p_count;
    m_updated = false;
}

ROC::ShaderUniform::~ShaderUniform()
{
    if(m_dataSize > 0U) delete[]m_data;
}

void ROC::ShaderUniform::SetData(const void *p_data, size_t p_dataSize)
{
    if(p_dataSize <= m_dataSize)
    {
        if(std::memcmp(m_data, p_data, p_dataSize) != 0)
        {
            std::memcpy(m_data, p_data, p_dataSize);
            m_updated = true;
        }
    }
}

const void* ROC::ShaderUniform::GetData() const
{
    return m_data;
}

GLint ROC::ShaderUniform::GetUniformName() const
{
    return m_uniformName;
}

unsigned char ROC::ShaderUniform::GetType() const
{
    return m_type;
}

size_t ROC::ShaderUniform::GetCount() const
{
    return m_count;
}

bool ROC::ShaderUniform::IsUpdated() const
{
    return m_updated;
}

void ROC::ShaderUniform::ResetUpdate()
{
    m_updated = false;
}

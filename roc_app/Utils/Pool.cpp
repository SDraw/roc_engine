#include "stdafx.h"

#include "Utils/Pool.h"

const size_t ROC::Pool::ms_invalid = std::numeric_limits<size_t>::max();

ROC::Pool::Pool(size_t p_size)
{
    m_size = p_size;
    m_poolData.assign(m_size, 0U);
    m_minimal = 0U;
}

ROC::Pool::~Pool()
{
    m_poolData.clear();
}

size_t ROC::Pool::Allocate()
{
    size_t l_allocated = ms_invalid;
    for(size_t i = m_minimal; i < m_size; i++)
    {
        if(m_poolData[i] == 0U)
        {
            m_poolData[i] = 1U;
            l_allocated = i;
            break;
        }
    }
    if(l_allocated == ms_invalid)
    {
        for(size_t i = 0; i < m_minimal; i++)
        {
            if(m_poolData[i] == 0U)
            {
                m_poolData[i] = 1U;
                l_allocated = i;
                break;
            }
        }
    }
    if(l_allocated != ms_invalid)
    {
        for(size_t i = l_allocated + 1U; i < m_size; i++)
        {
            if(m_poolData[i] == 0U)
            {
                m_minimal = i;
                break;
            }
        }
    }
    return l_allocated;
}

void ROC::Pool::Reset(size_t p_id)
{
    if(p_id < m_size)
    {
        m_poolData[p_id] = 0U;
        if(p_id < m_minimal) m_minimal = p_id;
    }
}

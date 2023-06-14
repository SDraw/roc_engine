#pragma once

namespace ROC
{

class Pool final
{
    size_t m_size;
    std::vector<unsigned char> m_poolData;
    size_t m_minimal;
public:
    static const size_t ms_invalid;

    explicit Pool(size_t p_size);
    ~Pool();

    size_t Allocate();
    void Reset(size_t p_id);
};

}

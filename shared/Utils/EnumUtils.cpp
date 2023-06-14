#include <vector>
#include <string>

#include "EnumUtils.h"

namespace EnumUtils
{

size_t ReadEnumVector(const std::string &p_val, const std::vector<std::string> &p_vec)
{
    size_t l_result = std::numeric_limits<size_t>::max();
    for(auto l_searchIter = p_vec.begin(), l_searchEnd = p_vec.end(); l_searchIter != l_searchEnd; ++l_searchIter)
    {
        if(!l_searchIter->compare(p_val))
        {
            l_result = std::distance(p_vec.begin(), l_searchIter);
            break;
        }
    }
    return l_result;
}

size_t ReadEnumVector(const char *p_val, const std::vector<std::string> &p_vec)
{
    size_t l_result = std::numeric_limits<size_t>::max();
    for(auto l_searchIter = p_vec.begin(), l_searchEnd = p_vec.end(); l_searchIter != l_searchEnd; ++l_searchIter)
    {
        if(!l_searchIter->compare(p_val))
        {
            l_result = std::distance(p_vec.begin(), l_searchIter);
            break;
        }
    }
    return l_result;
}

}

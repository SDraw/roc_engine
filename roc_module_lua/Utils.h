#pragma once

namespace Utils
{

bool IsValid(ROC::IElement *p_obj);

void DeleteByType(void *p_obj, const std::string &p_type);

template<class T, class U> inline T Cast(U p_obj)
{
    return dynamic_cast<T>(p_obj);
}

}


#include "stdafx.h"

#include "Utils/MathUtils.h"

namespace MathUtils
{

bool IsPowerOfTwo(int p_value)
{
    return (p_value > 0 && ((p_value & (p_value - 1)) == 0));
}

float EaseInOut(float p_value)
{
    return -0.5f*(cos(glm::pi<float>()*p_value) - 1.f);
}

int Power(int p_value, int p_exp)
{
    int l_result = 1;
    for(int i = 1; i <= p_exp; i++)
        l_result *= p_value;
    return l_result;
}

}

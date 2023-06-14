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

}

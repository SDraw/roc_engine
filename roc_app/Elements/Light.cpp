#include "stdafx.h"

#include "Elements/Light.h"

namespace ROC
{

extern const glm::vec2 g_emptyVec2;
extern const glm::vec3 g_emptyVec3;
extern const glm::vec3 g_defaultDirection;

}

ROC::Light::Light(unsigned char p_type)
{
    m_elementType = ET_Light;

    m_direction = g_defaultDirection;
    m_color = glm::vec4(1.f);
    m_cutoff = g_emptyVec2;
    m_falloff = glm::vec3(1.f, 0.f, 0.f);

    m_type = glm::clamp<unsigned char>(p_type, LT_Directional, LT_Spotlight);
}

void ROC::Light::SetDirection(const glm::vec3 &p_vec)
{
    std::memcpy(&m_direction, &p_vec, sizeof(glm::vec3));
}

const glm::vec3& ROC::Light::GetDirection() const
{
    return m_direction;
}

void ROC::Light::SetColor(const glm::vec4 &p_vec)
{
    std::memcpy(&m_color, &p_vec, sizeof(glm::vec4));
}

const glm::vec4& ROC::Light::GetColor() const
{
    return m_color;
}

void ROC::Light::SetCutoff(const glm::vec2 &p_vec)
{
    std::memcpy(&m_cutoff, &p_vec, sizeof(glm::vec2));
}

const glm::vec2& ROC::Light::GetCutoff() const
{
    return m_cutoff;
}

void ROC::Light::SetFalloff(const glm::vec3 &p_val)
{
    std::memcpy(&m_falloff, &p_val, sizeof(glm::vec3));
}

const glm::vec3& ROC::Light::GetFalloff() const
{
    return m_falloff;
}

unsigned char ROC::Light::GetType() const
{
    return m_type;
}

void ROC::Light::SetType(unsigned char p_type)
{
    m_type = glm::clamp<unsigned char>(p_type, LT_Directional, LT_Spotlight);
}

// ROC::Transformable
void ROC::Light::SetRotation(const glm::quat &p_rot)
{
    m_direction = p_rot * g_defaultDirection;
    
    Transformable::SetRotation(p_rot);
}

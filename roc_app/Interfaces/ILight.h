#pragma once

namespace ROC
{

class ILight
{
public:
    enum LightType : unsigned char
    {
        LT_Directional = 0U,
        LT_Point,
        LT_Spotlight
    };

    virtual void SetDirection(const glm::vec3 &p_vec) = 0;
    virtual const glm::vec3& GetDirection() const = 0;
    virtual void SetColor(const glm::vec4 &p_vec) = 0;
    virtual const glm::vec4& GetColor() const = 0;
    virtual void SetCutoff(const glm::vec2 &p_vec) = 0;
    virtual const glm::vec2& GetCutoff() const = 0;
    virtual void SetFalloff(const glm::vec3 &p_val) = 0;
    virtual const glm::vec3& GetFalloff() const = 0;
    virtual unsigned char GetType() const = 0;
    virtual void SetType(unsigned char p_type) = 0;
};

}

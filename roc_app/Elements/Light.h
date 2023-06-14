#pragma once
#include "Interfaces/ILight.h"
#include "Elements/Element.h"
#include "Elements/Transformable.h"

namespace ROC
{

class Light final : public Element, public Transformable, public ILight
{
    glm::vec3 m_direction;
    glm::vec4 m_color;
    glm::vec2 m_cutoff;
    glm::vec3 m_falloff;
    unsigned char m_type;

    Light(const Light &that) = delete;
    Light& operator=(const Light &that) = delete;
public:
    explicit Light(unsigned char p_type);
    ~Light() = default;

    void SetDirection(const glm::vec3 &p_vec);
    const glm::vec3& GetDirection() const;

    void SetColor(const glm::vec4 &p_vec);
    const glm::vec4& GetColor() const;

    void SetCutoff(const glm::vec2 &p_vec);
    const glm::vec2& GetCutoff() const;

    void SetFalloff(const glm::vec3 &p_val);
    const glm::vec3& GetFalloff() const;

    unsigned char GetType() const;
    void SetType(unsigned char p_type);

    // ROC::Transformable
    void SetRotation(const glm::quat &p_rot) override;
};

}

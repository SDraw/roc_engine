#include "stdafx.h"
#include "Elements/Transformable.h"

ROC::Transformable::Transformable()
{
    m_transform = new Transformation();
}

ROC::Transformable::~Transformable()
{
    delete m_transform;
}

void ROC::Transformable::SetPosition(const glm::vec3 &p_pos)
{
    m_transform->SetPosition(p_pos);
}

const glm::vec3& ROC::Transformable::GetPosition() const
{
    return m_transform->GetPosition();
}

void ROC::Transformable::SetRotation(const glm::quat &p_rot)
{
    m_transform->SetRotation(p_rot);
}

const glm::quat& ROC::Transformable::GetRotation() const
{
    return m_transform->GetRotation();
}

void ROC::Transformable::SetScale(const glm::vec3 &p_scl)
{
    m_transform->SetScale(p_scl);
}

const glm::vec3& ROC::Transformable::GetScale() const
{
    return m_transform->GetScale();
}

const glm::mat4& ROC::Transformable::GetMatrix() const
{
    return m_transform->GetMatrix();
}

void ROC::Transformable::UpdateMatrix()
{
    m_transform->UpdateMatrix();
}

bool ROC::Transformable::NeedsUpdate() const
{
    return m_transform->NeedsUpdate();
}

bool ROC::Transformable::IsUpdated() const
{
    return m_transform->IsUpdated();
}

bool ROC::Transformable::IsScaled() const
{
    return m_transform->IsScaled();
}



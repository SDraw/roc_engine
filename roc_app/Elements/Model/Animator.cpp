#include "stdafx.h"

#include "Elements/Model/Animator.h"
#include "Elements/Animation/Animation.h"
#include "Elements/Model/Bone.h"

#include "Utils/SystemTick.h"

ROC::Animator::Animator()
{
    m_animation = nullptr;
    m_tick = 0U;
    m_state = AS_None;
    m_speed = 1.f;
    m_blend = true;
    m_blendTime = 500U;
    m_blendTimeTick = 0U;
    m_blendValue = 1.f;
    m_loop = false;
}

void ROC::Animator::SetAnimation(Animation *p_anim)
{
    m_animation = p_anim;
    if(m_animation)
    {
        m_tick = 0U;
        m_blend = true;
        m_blendTimeTick = 0U;
        if(m_state == AS_None) m_state = AS_Paused;
        m_loop = false;
    }
    else m_state = AS_None;
}

ROC::Animation* ROC::Animator::GetAnimation() const
{
    return m_animation;
}

bool ROC::Animator::Play(bool p_loop)
{
    if(m_animation)
    {
        m_loop = p_loop;
        m_state = AS_Playing;
    }
    return (m_animation != nullptr);
}

bool ROC::Animator::IsPlaying() const
{
    return (m_state == AS_Playing);
}

bool ROC::Animator::Pause()
{
    if(m_animation) m_state = AS_Paused;
    return (m_animation != nullptr);
}

bool ROC::Animator::IsPaused() const
{
    return (m_state == AS_Paused);
}

bool ROC::Animator::Reset()
{
    if(m_animation) m_tick = 0U;
    return (m_animation != nullptr);
}

bool ROC::Animator::SetSpeed(float p_speed)
{
    if(m_animation)
    {
        m_speed = glm::clamp(p_speed, 0.f, std::numeric_limits<float>::max());
    }
    return (m_animation != nullptr);
}

float ROC::Animator::GetSpeed() const
{
    return m_speed;
}

bool ROC::Animator::SetProgress(float p_val)
{
    if(m_animation)
    {
        p_val = glm::clamp(p_val, 0.f, 1.f);
        m_tick = static_cast<unsigned int>(static_cast<float>(m_animation->GetDuration())*p_val);
    }
    return (m_animation != nullptr);
}

float ROC::Animator::GetProgress() const
{
    float l_result = 0.f;
    if(m_animation) l_result = (static_cast<float>(m_tick) / static_cast<float>(m_animation->GetDuration()));
    return l_result;
}

unsigned int ROC::Animator::GetTick() const
{
    return m_tick;
}

bool ROC::Animator::SetBlendTime(unsigned int p_val)
{
    if(m_animation)
    {
        m_blendTime = glm::clamp(p_val, 1U, std::numeric_limits<unsigned int>::max());
    }
    return (m_animation != nullptr);
}

unsigned int ROC::Animator::GetBlendTime() const
{
    return m_blendTime;
}

float ROC::Animator::GetBlendValue() const
{
    return m_blendValue;
}

void ROC::Animator::Update()
{
    if(m_animation && (m_state == AS_Playing))
    {
        m_tick += static_cast<unsigned int>(static_cast<float>(SystemTick::GetDelta())*m_speed);
        if(m_loop) m_tick %= m_animation->GetDuration();
        else
        {
            if(m_tick >= m_animation->GetDuration())
            {
                m_tick = m_animation->GetDuration();
                m_state = AS_Paused;
            }
        }
        if(m_blend)
        {
            m_blendTimeTick += static_cast<unsigned int>(SystemTick::GetDelta());
            if(m_blendTimeTick >= m_blendTime)
            {
                m_blendTimeTick = m_blendTime;
                m_blend = false;
                m_blendValue = 1.f;
            }
            else m_blendValue = static_cast<float>(m_blendTimeTick) / static_cast<float>(m_blendTime);
        }
    }
}

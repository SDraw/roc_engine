#include "stdafx.h"

#include "Elements/Sound.h"

namespace ROC
{

extern const glm::vec2 g_emptyVec2;
extern const glm::vec3 g_emptyVec3;

}

ROC::Sound::Sound()
{
    m_elementType = ET_Sound;

    m_handle = nullptr;
    m_relative = false;
    m_looped = false;
    m_mono = false;
    m_v3DPosition = g_emptyVec3;
    m_v3DDistance = g_emptyVec2;
}

ROC::Sound::~Sound()
{
    delete m_handle;
}

bool ROC::Sound::Load(const std::string &p_path)
{
    if(!m_handle)
    {
        m_handle = new sf::Music;
        if(m_handle->openFromFile(p_path)) m_mono = (m_handle->getChannelCount() == 1);
        else
        {
            delete m_handle;
            m_handle = nullptr;
        }
    }
    return (m_handle != nullptr);
}

void ROC::Sound::Play()
{
    if(m_handle) m_handle->play();
}

void ROC::Sound::Pause()
{
    if(m_handle) m_handle->pause();
}

void ROC::Sound::Stop()
{
    if(m_handle) m_handle->stop();
}

bool  ROC::Sound::IsLooped() const
{
    return m_looped;
}

bool ROC::Sound::SetLoop(bool p_loop)
{
    if(m_handle && (m_looped != p_loop))
    {
        m_looped = p_loop;
        m_handle->setLoop(m_looped);
    }
    return ((m_handle != nullptr) && (m_looped == p_loop));
}

float ROC::Sound::GetDuration()
{
    float l_duration = -1.f;
    if(m_handle) l_duration = m_handle->getDuration().asSeconds();
    return l_duration;
}

void ROC::Sound::SetSpeed(float p_speed)
{
    if(m_handle)
    {
        p_speed = glm::clamp(p_speed, 0.f, std::numeric_limits<float>::max());
        m_handle->setPitch(p_speed);
    }
}

float ROC::Sound::GetSpeed()
{
    float l_speed = -1.f;
    if(m_handle) l_speed = m_handle->getPitch();
    return l_speed;
}

void ROC::Sound::SetVolume(float p_volume)
{
    if(m_handle)
    {
        p_volume = glm::clamp(p_volume, 0.f, 100.f);
        m_handle->setVolume(p_volume);
    }
}

float ROC::Sound::GetVolume()
{
    float l_volume = -1.f;
    if(m_handle) l_volume = m_handle->getVolume();
    return l_volume;
}

void ROC::Sound::SetTime(float p_time)
{
    if(m_handle)
    {
        p_time = glm::clamp(p_time, 0.f, std::numeric_limits<float>::max());
        sf::Time l_time = sf::seconds(p_time);
        m_handle->setPlayingOffset(l_time);
    }
}

float ROC::Sound::GetTime()
{
    float l_time = -1.f;
    if(m_handle) l_time = m_handle->getPlayingOffset().asSeconds();
    return l_time;
}

bool ROC::Sound::Set3DPositionEnabled(bool p_state)
{
    if(m_handle)
    {
        if(m_mono)
        {
            if(m_relative != p_state)
            {
                m_relative = p_state;
                m_handle->setRelativeToListener(!m_relative);
                if(m_relative)
                {
                    m_handle->setPosition(m_v3DPosition.x, m_v3DPosition.y, m_v3DPosition.z);
                    m_handle->setMinDistance(m_v3DDistance.x);
                    m_handle->setAttenuation(m_v3DDistance.y);
                }
                else
                {
                    std::memcpy(&m_v3DPosition, &g_emptyVec3, sizeof(glm::vec3));
                    m_v3DDistance.x = m_v3DDistance.y = 0.f;
                }
            }
        }
    }
    return m_mono;
}

bool ROC::Sound::Get3DPositionEnabled() const
{
    return m_relative;
}

bool ROC::Sound::Set3DPosition(const glm::vec3 &p_pos)
{
    if(m_handle && m_relative)
    {
        std::memcpy(&m_v3DPosition, &p_pos, sizeof(glm::vec3));
        m_handle->setPosition(m_v3DPosition.x, m_v3DPosition.y, m_v3DPosition.z);
    }
    return m_relative;
}

const glm::vec3& ROC::Sound::Get3DPosition(glm::vec3 &p_pos) const
{
    return m_v3DPosition;
}

bool ROC::Sound::Set3DDistance(const glm::vec2 &p_dist)
{
    if(m_handle && m_relative)
    {
        std::memcpy(&m_v3DDistance, &p_dist, sizeof(glm::vec2));
        m_handle->setMinDistance(m_v3DDistance.x);
        m_handle->setAttenuation(m_v3DDistance.y);
    }
    return m_relative;
}

const glm::vec2& ROC::Sound::Get3DDistance(glm::vec2 &p_dist) const
{
    return m_v3DDistance;
}

int ROC::Sound::GetState() const
{
    int l_state = -1;
    if(m_handle) l_state = m_handle->getStatus();
    return l_state;
}

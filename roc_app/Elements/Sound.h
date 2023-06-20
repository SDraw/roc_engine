#pragma once
#include "Interfaces/ISound.h"
#include "Elements/Element.h"

namespace ROC
{

class Sound final : public Element, public ISound
{
    sf::Music *m_handle;

    bool m_relative;
    bool m_looped;
    bool m_mono;

    glm::vec3 m_v3DPosition;
    glm::vec2 m_v3DDistance;

    Sound(const Sound &that) = delete;
    Sound& operator=(const Sound &that) = delete;
public:
    explicit Sound();
    ~Sound();

    bool Load(const std::string &p_path);

    void Play();
    void Pause();
    void Stop();

    bool IsLooped() const;
    bool SetLoop(bool p_loop);

    float GetDuration();

    void SetSpeed(float p_speed);
    float GetSpeed();

    void SetVolume(float p_volume);
    float GetVolume();

    void SetTime(float p_time);
    float GetTime();

    bool Set3DPositionEnabled(bool p_state);
    bool Get3DPositionEnabled() const;

    bool Set3DPosition(const glm::vec3 &p_pos);
    const glm::vec3& Get3DPosition() const;

    bool Set3DDistance(const glm::vec2 &p_dist);
    const glm::vec2& Get3DDistance() const;

    int GetState() const;
};

}

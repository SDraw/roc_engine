#pragma once

namespace ROC
{

class ISound
{
public:
    virtual void Play() = 0;
    virtual void Pause() = 0;
    virtual void Stop() = 0;
    virtual bool IsLooped() const = 0;
    virtual bool SetLoop(bool p_loop) = 0;
    virtual float GetDuration() = 0;
    virtual void SetSpeed(float p_speed) = 0;
    virtual float GetSpeed() = 0;
    virtual void SetVolume(float p_volume) = 0;
    virtual float GetVolume() = 0;
    virtual void SetTime(float p_time) = 0;
    virtual float GetTime() = 0;
    virtual bool Set3DPositionEnabled(bool p_state) = 0;
    virtual bool Get3DPositionEnabled() const = 0;
    virtual bool Set3DPosition(const glm::vec3 &p_pos) = 0;
    virtual const glm::vec3& Get3DPosition(glm::vec3 &p_pos) const = 0;
    virtual bool Set3DDistance(const glm::vec2 &p_dist) = 0;
    virtual const glm::vec2& Get3DDistance(glm::vec2 &p_dist) const = 0;
    virtual int GetState() const = 0;
};

}

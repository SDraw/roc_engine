#pragma once

namespace ROC
{

class Animation;
class Bone;

class Animator final
{
    enum AnimationState : unsigned char
    {
        AS_None = 0U,
        AS_Paused,
        AS_Playing
    };

    Animation *m_animation;
    unsigned int m_tick;
    AnimationState m_state;
    float m_speed;
    bool m_loop;

    bool m_blend;
    unsigned int m_blendTime;
    unsigned int m_blendTimeTick;
    float m_blendValue;

    Animator(const Animator &that) = delete;
    Animator& operator=(const Animator &that) = delete;
public:
    Animator();
    ~Animator() = default;

    bool Play(bool p_loop);
    bool Pause();
    bool Reset();

    bool IsPlaying() const;
    bool IsPaused() const;

    bool SetSpeed(float p_speed);
    float GetSpeed() const;

    bool SetProgress(float p_val);
    float GetProgress() const;

    bool SetBlendTime(unsigned int p_val);
    unsigned int GetBlendTime() const;

    void SetAnimation(Animation *p_anim);
    Animation* GetAnimation() const;

    unsigned int GetTick() const;
    float GetBlendValue() const;

    void Update();
};

}

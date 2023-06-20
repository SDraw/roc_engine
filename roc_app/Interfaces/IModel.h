#pragma once

namespace ROC
{

class IAnimation;
class ICollider;
class IMesh;

class IModel
{
protected:
    ~IModel() = default;
public:
    enum ModelAnimationProperty : unsigned char
    {
        MAP_Speed = 0U,
        MAP_Progress,
        Map_BlendTime
    };

    virtual IMesh* GetIMesh() const = 0;
    virtual float GetBoundSphereRadius() const = 0;
    virtual bool SetIAnimation(IAnimation *p_anim) = 0;
    virtual bool RemoveAnimation() = 0;
    virtual IAnimation* GetIAnimation() const = 0;
    virtual bool PlayAnimation(bool p_loop) = 0;
    virtual bool PauseAnimation() = 0;
    virtual bool ResetAnimation() = 0;
    virtual bool GetAnimationProperty(ModelAnimationProperty p_prop, float &p_value) = 0;
    virtual bool SetAnimationProperty(ModelAnimationProperty p_prop, float p_value) = 0;
    virtual bool IsVisible() const = 0;
};

}

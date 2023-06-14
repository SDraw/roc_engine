#pragma once
#include "Interfaces/IModel.h"
#include "Elements/Element.h"
#include "Elements/Collidable.h"
#include "Elements/Transformable.h"

namespace ROC
{

class Animation;
class Animator;
class Bone;
class Collision;
class Mesh;
class Skeleton;
class Transformation;

class Model final : public Element, public Collidable, public Transformable, public IModel
{
    Mesh *m_mesh;
    float m_boundSphereRaduis;

    bool m_visibility;
    float m_visibilityDistance;

    Animator *m_animator;
    Skeleton *m_skeleton;

    Model(const Model &that) = delete;
    Model& operator=(const Model &that) = delete;

    // ROC::Element
    void OnParentRemoved(Element *p_parent) override;

    // ROC::Collidable
    void GetRigidBodies(std::vector<btRigidBody*> &p_vec);

    // ROC::IModel
    IMesh* GetIMesh() const;
    bool SetIAnimation(IAnimation *p_anim);
    IAnimation* GetIAnimation() const;
public:
    enum ModelUpdateStage : unsigned char
    {
        MUS_Matrix = 0U,
        MUS_SkeletonStatic,
        MUS_SkeletonDynamic
    };

    explicit Model(Mesh *p_mesh);
    ~Model();

    Mesh* GetMesh() const;
    float GetBoundSphereRadius() const;

    bool SetAnimation(Animation *p_anim);
    bool RemoveAnimation();
    Animation* GetAnimation() const;
    bool PlayAnimation(bool p_loop);
    bool PauseAnimation();
    bool ResetAnimation();
    bool GetAnimationProperty(ModelAnimationProperty p_prop, float &p_value);
    bool SetAnimationProperty(ModelAnimationProperty p_prop, float p_value);

    Skeleton* GetSkeleton() const;

    void SetVisibility(bool p_state);
    bool IsVisible() const;

    void SetVisibilityDistance(float p_dist);
    float GetVisibilityDistance() const;

    void Update(ModelUpdateStage p_stage);
};

}

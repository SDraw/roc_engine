#pragma once
namespace ROC
{

class BoneFrameData final
{
public:
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
    bool m_useScale;

    BoneFrameData();
    BoneFrameData(const glm::vec3 &p_pos, const glm::quat &p_rot, const glm::vec3 &p_scl);
    ~BoneFrameData();

    bool IsEqual(const BoneFrameData *p_data) const;

    void SetInterpolated(BoneFrameData *p_data, float p_blend);
    void SetInterpolated(BoneFrameData *p_leftData, BoneFrameData *p_rightData, float p_blend);
};

}

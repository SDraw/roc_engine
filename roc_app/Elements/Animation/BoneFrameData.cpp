#include "stdafx.h"

#include "Elements/Animation/BoneFrameData.h"

namespace ROC
{

extern const glm::vec3 g_emptyVec3;
extern const glm::quat g_defaultRotation;
extern const glm::vec3 g_defaultScale;
extern const float g_epsilon;

}

ROC::BoneFrameData::BoneFrameData()
{
    m_position = g_emptyVec3;
    m_rotation = g_defaultRotation;
    m_scale = g_defaultScale;
    m_useScale = false;
}

ROC::BoneFrameData::BoneFrameData(const glm::vec3 &p_pos, const glm::quat &p_rot, const glm::vec3 &p_scl)
{
    m_position = p_pos;
    m_rotation = p_rot;
    m_scale = p_scl;
    m_useScale = glm::all(glm::epsilonNotEqual(m_scale, g_defaultScale, g_epsilon));
}

ROC::BoneFrameData::~BoneFrameData()
{
}

bool ROC::BoneFrameData::IsEqual(const BoneFrameData *p_data) const
{
    const bool l_positionResult = (m_position == p_data->m_position);
    const bool l_rotationResult = (m_rotation == p_data->m_rotation);
    const bool l_scaleResult = (m_scale == p_data->m_scale);
    return (l_positionResult && l_rotationResult && l_scaleResult);
}

void ROC::BoneFrameData::SetInterpolated(BoneFrameData *p_data, float p_blend)
{
    m_position = glm::mix(m_position, p_data->m_position, p_blend);
    m_rotation = glm::slerp(m_rotation, p_data->m_rotation, p_blend);
    m_scale = glm::mix(m_scale, p_data->m_scale, p_blend);
    m_useScale = (m_useScale || p_data->m_useScale);
}

void ROC::BoneFrameData::SetInterpolated(BoneFrameData *p_leftData, BoneFrameData *p_rightData, float p_blend)
{
    m_position = glm::mix(p_leftData->m_position, p_rightData->m_position, p_blend);
    m_rotation = glm::slerp(p_leftData->m_rotation, p_rightData->m_rotation, p_blend);
    m_scale = glm::mix(p_leftData->m_scale, p_rightData->m_scale, p_blend);
    m_useScale = (p_leftData->m_useScale || p_rightData->m_useScale);
}

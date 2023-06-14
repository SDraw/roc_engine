#include "stdafx.h"

#include "Elements/Animation/Animation.h"
#include "Elements/Animation/BoneFrameData.h"

#include "Elements/Model/Bone.h"
#include "Utils/MathUtils.h"

ROC::Animation::Animation()
{
    m_elementType = ET_Animation;

    m_framesCount = 0U;
    m_duration = 0U;
    m_fps = 0U;
    m_frameDelta = 0U;
    m_tempFrameData = nullptr;
    m_bonesCount = 0U;
    m_loaded = false;
}

ROC::Animation::~Animation()
{
    Clean();
}

void ROC::Animation::Clean()
{
    m_bonesCount = 0U;
    m_framesCount = 0U;
    m_duration = 0U;
    m_fps = 0U;
    delete m_tempFrameData;
    for(auto &l_boneInterval : m_boneIntervals)
    {
        for(size_t i = 0, j = l_boneInterval.size(); i < j; i++)
        {
            if(i == 0U) delete l_boneInterval[i].m_leftData;
            else delete l_boneInterval[i].m_rightData;
        }
        l_boneInterval.clear();
    }
    m_boneIntervals.clear();
    m_loaded = false;
}

bool ROC::Animation::Load(const std::string &p_path)
{
    if(!m_loaded)
    {
        std::ifstream l_animFile;
        l_animFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            l_animFile.open(p_path, std::ios::binary | std::ios::in);
            l_animFile.read(reinterpret_cast<char*>(&m_fps), sizeof(unsigned int));
            l_animFile.read(reinterpret_cast<char*>(&m_framesCount), sizeof(unsigned int));
            l_animFile.read(reinterpret_cast<char*>(&m_bonesCount), sizeof(unsigned int));

            m_frameDelta = 1000U / m_fps;
            m_duration = m_framesCount * m_frameDelta;

            for(unsigned int i = 0; i < m_bonesCount; i++)
            {
                std::vector<keyframeData> l_intervals;
                keyframeData l_interval;

                unsigned int l_intervalsCount = 0;
                l_animFile.read(reinterpret_cast<char*>(&l_intervalsCount), sizeof(unsigned int));

                for(unsigned int j = 0; j < l_intervalsCount; j++)
                {
                    glm::vec3 l_position;
                    glm::quat l_rotation;
                    glm::vec3 l_scale;
                    unsigned int l_frameIndex;
                    l_animFile.read(reinterpret_cast<char*>(&l_position), sizeof(glm::vec3));
                    l_animFile.read(reinterpret_cast<char*>(&l_rotation), sizeof(glm::quat));
                    l_animFile.read(reinterpret_cast<char*>(&l_scale), sizeof(glm::vec3));
                    l_animFile.read(reinterpret_cast<char*>(&l_frameIndex), sizeof(unsigned int));

                    if(j == 0) l_interval.m_leftData = new BoneFrameData(l_position, l_rotation, l_scale);
                    else
                    {
                        l_interval.m_rightData = new BoneFrameData(l_position, l_rotation, l_scale);
                        l_interval.m_duration = l_frameIndex*m_frameDelta - l_interval.m_startTime;
                        l_interval.m_static = l_interval.m_leftData->IsEqual(l_interval.m_rightData);
                        l_intervals.push_back(l_interval);

                        std::swap(l_interval.m_leftData, l_interval.m_rightData);
                        l_interval.m_startTime = l_frameIndex*m_frameDelta;
                        l_interval.m_rightData = nullptr;
                    }
                }
                l_intervals.shrink_to_fit();
                m_boneIntervals.push_back(l_intervals);
            }
            m_boneIntervals.shrink_to_fit();
            l_animFile.close();

            m_tempFrameData = new BoneFrameData();
            m_loaded = true;
        }
        catch(const std::exception&)
        {
            Clean();
        }
    }
    return m_loaded;
}

size_t ROC::Animation::GetBonesCount() const
{
    return static_cast<size_t>(m_bonesCount);
}

unsigned int ROC::Animation::GetDuration() const
{
    return m_duration;
}

void ROC::Animation::GetData(unsigned int p_tick, std::vector<Bone*> &p_bones, float p_blend)
{
    if(m_loaded)
    {
        const bool l_enableBlending = glm::epsilonEqual(p_blend,1.f,0.000001f);
        p_tick = p_tick%m_duration;

        for(size_t i = 0U; i < static_cast<size_t>(m_bonesCount); i++)
        {
            // Binary search
            if(!m_boneIntervals[i].empty())
            {
                size_t l_left = 0U;
                size_t l_right = m_boneIntervals[i].size() - 1U;
                while(l_right - l_left > 0U)
                {
                    const auto &l_leftFrame = m_boneIntervals[i][l_left];
                    if(l_leftFrame.m_startTime + l_leftFrame.m_duration >= p_tick)
                    {
                        l_right = l_left;
                        break;
                    }

                    const auto &l_rightFrame = m_boneIntervals[i][l_right];
                    if(l_rightFrame.m_startTime <= p_tick)
                    {
                        l_left = l_right;
                        break;
                    }

                    size_t l_mid = l_left + (l_right - l_left) / 2U;
                    const auto &l_midFrame = m_boneIntervals[i][l_mid];
                    if(l_midFrame.m_startTime <= p_tick) l_left = l_mid;
                    else l_right = l_mid;
                }

                const auto &l_keyframeData = m_boneIntervals[i][l_left];
                if(l_keyframeData.m_static)
                {
                    p_bones[i]->SetFrameData(l_keyframeData.m_leftData);
                    l_enableBlending ? p_bones[i]->SetFrameData(l_keyframeData.m_leftData, p_blend) : p_bones[i]->SetFrameData(l_keyframeData.m_leftData);
                }
                else
                {
                    const float l_blend = MathUtils::EaseInOut(static_cast<float>(p_tick - l_keyframeData.m_startTime) / static_cast<float>(l_keyframeData.m_duration));
                    m_tempFrameData->SetInterpolated(l_keyframeData.m_leftData, l_keyframeData.m_rightData, l_blend);
                    l_enableBlending ? p_bones[i]->SetFrameData(m_tempFrameData, p_blend) : p_bones[i]->SetFrameData(m_tempFrameData);
                }
            }
        }
    }
}

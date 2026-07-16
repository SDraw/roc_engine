using GlmSharp;
using System;
using System.IO;

namespace ROC.Engine.Objects.Resources
{
    public sealed class Animation : Resource
    {
        internal struct KeyframeData
        {
            public vec3 m_position;
            public quat m_rotation;
            public vec3 m_scale;
            public uint m_frameIndex;
            public uint m_time;
        }

        internal struct BoneAnimation
        {
            public uint m_boneIndex;
            public KeyframeData[] m_keyframes;
        }

        internal struct AnimationFrame
        {
            public KeyframeData m_keyframe;
            public bool m_available;
        }

        uint m_fps = 0U;
        uint m_framesCount = 0U;
        int m_bonesCount = 0;

        uint m_frameDelta = 0U;
        uint m_duration = 0U;

        BoneAnimation[] m_boneAnimationData = null;
        AnimationFrame[] m_animationData = null;

        public uint FPS => m_fps;
        public uint FrameDelta => m_frameDelta;
        public uint Duration => m_duration;
        public int BonesCount => m_bonesCount;
        public uint FramesCount => m_framesCount;

        internal Animation()
        {
        }

        internal void Load(string p_path)
        {
            if(m_loaded)
                return;

            try
            {
                FileStream l_file = new FileStream(p_path, FileMode.Open, FileAccess.Read);
                BinaryReader l_reader = new BinaryReader(l_file);

                m_fps = l_reader.ReadUInt32();
                m_framesCount = l_reader.ReadUInt32();
                m_bonesCount = (int)l_reader.ReadUInt32();

                m_frameDelta = 1000U / m_fps;
                m_duration = m_framesCount * m_frameDelta;

                m_boneAnimationData = new BoneAnimation[m_bonesCount];
                m_animationData = new AnimationFrame[m_bonesCount];
                for(uint i = 0; i < m_bonesCount; i++)
                {
                    m_boneAnimationData[i] = new BoneAnimation();
                    m_boneAnimationData[i].m_boneIndex = i;

                    m_animationData[i] = new AnimationFrame();

                    uint l_keyframesCount = l_reader.ReadUInt32();
                    m_boneAnimationData[i].m_keyframes = new KeyframeData[l_keyframesCount];

                    for(uint j = 0; j < l_keyframesCount; j++)
                    {
                        m_boneAnimationData[i].m_keyframes[j] = new KeyframeData();
                        m_boneAnimationData[i].m_keyframes[j].m_position.x = l_reader.ReadSingle();
                        m_boneAnimationData[i].m_keyframes[j].m_position.y = l_reader.ReadSingle();
                        m_boneAnimationData[i].m_keyframes[j].m_position.z = l_reader.ReadSingle();

                        m_boneAnimationData[i].m_keyframes[j].m_rotation.x = l_reader.ReadSingle();
                        m_boneAnimationData[i].m_keyframes[j].m_rotation.y = l_reader.ReadSingle();
                        m_boneAnimationData[i].m_keyframes[j].m_rotation.z = l_reader.ReadSingle();
                        m_boneAnimationData[i].m_keyframes[j].m_rotation.w = l_reader.ReadSingle();

                        m_boneAnimationData[i].m_keyframes[j].m_rotation = m_boneAnimationData[i].m_keyframes[j].m_rotation.NormalizedSafe;

                        m_boneAnimationData[i].m_keyframes[j].m_scale.x = l_reader.ReadSingle();
                        m_boneAnimationData[i].m_keyframes[j].m_scale.y = l_reader.ReadSingle();
                        m_boneAnimationData[i].m_keyframes[j].m_scale.z = l_reader.ReadSingle();
                        m_boneAnimationData[i].m_keyframes[j].m_frameIndex = l_reader.ReadUInt32();

                        m_boneAnimationData[i].m_keyframes[j].m_time = m_boneAnimationData[i].m_keyframes[j].m_frameIndex * m_frameDelta;
                    }
                }

                m_loaded = true;
            }
            catch(Exception e)
            {
                m_log = e.Message;

                m_boneAnimationData = null;
                m_bonesCount = 0;
                m_framesCount = 0U;
                m_duration = 0U;
                m_fps = 0U;
            }
        }

        public override void Unload()
        {
            if(!m_loaded)
                return;

            m_boneAnimationData = null;
            m_bonesCount = 0;
            m_framesCount = 0U;
            m_duration = 0U;
            m_fps = 0U;

            m_loaded = false;
        }

        // Animation
        // Just simple loop for searching frame, no search optimizations
        internal AnimationFrame[] GetFrameData(uint p_time)
        {
            if(!m_loaded)
                return new AnimationFrame[0];

            p_time %= m_duration;

            for(int i = 0; i < m_bonesCount; i++)
            {
                m_animationData[i].m_available = false;
                for(int j1 = 0, k = m_boneAnimationData[i].m_keyframes.Length; j1 < k; j1++)
                {
                    if(p_time < m_boneAnimationData[i].m_keyframes[j1].m_time)
                    {
                        // Found our frame
                        m_animationData[i].m_available = true;

                        int j0 = j1 - 1;
                        if(j0 > -1)
                        {
                            // Interpolate
                            float l_blend = EaseInOut(InverseLerp(m_boneAnimationData[i].m_keyframes[j0].m_time, m_boneAnimationData[i].m_keyframes[j1].m_time, p_time));
                            m_animationData[i].m_keyframe.m_position = vec3.Lerp(m_boneAnimationData[i].m_keyframes[j0].m_position, m_boneAnimationData[i].m_keyframes[j1].m_position, l_blend);
                            m_animationData[i].m_keyframe.m_rotation = SLerpSafe(m_boneAnimationData[i].m_keyframes[j0].m_rotation, m_boneAnimationData[i].m_keyframes[j1].m_rotation, l_blend);
                            m_animationData[i].m_keyframe.m_scale = vec3.Lerp(m_boneAnimationData[i].m_keyframes[j0].m_scale, m_boneAnimationData[i].m_keyframes[j1].m_scale, l_blend);
                        }
                        else
                        {
                            // Single keyframe
                            m_animationData[i].m_keyframe.m_position = m_boneAnimationData[i].m_keyframes[j1].m_position;
                            m_animationData[i].m_keyframe.m_rotation = m_boneAnimationData[i].m_keyframes[j1].m_rotation;
                            m_animationData[i].m_keyframe.m_scale = m_boneAnimationData[i].m_keyframes[j1].m_scale;
                        }

                        break;
                    }
                }
            }

            return m_animationData;
        }

        // Utils
        static float InverseLerp(uint a, uint b, uint t)
        {
            float l_up = t - a;
            float l_down = b - a;
            return l_up / l_down;
        }

        static quat SLerpSafe(quat a, quat b, float blend)
        {
            if(a == b)
                return a;

            return quat.SLerp(a, b, blend);
        }

        static float EaseInOut(float p_value) => -0.5f * ((float)Math.Cos(Math.PI * p_value) - 1f);

        // API
        public static Animation Import(string p_path)
        {
            Animation l_anim = new Animation();
            l_anim.Load(p_path);
            return l_anim;
        }
    }
}

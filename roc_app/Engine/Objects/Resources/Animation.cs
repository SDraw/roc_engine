using GlmSharp;
using System;
using System.IO;

namespace ROC.Engine.Objects.Resources
{
    public sealed class Animation : Resource
    {
        internal struct BoneAnimation
        {
            public Timeline<vec3> m_positionsTimeline;
            public Timeline<quat> m_rotationsTimeline;
            public Timeline<vec3> m_scalesTimeline;
        }

        internal struct AnimationFrame
        {
            public vec3 m_position;
            public quat m_rotation;
            public vec3 m_scale;
            public bool m_hasPosition;
            public bool m_hasRotation;
            public bool m_hasScale;
        }

        static AnimationFrame[] ms_emptyAnimationFrame = new AnimationFrame[0];

        int m_bonesCount = 0;
        uint m_duration = 0U;

        BoneAnimation[] m_boneAnimationData = null;
        AnimationFrame[] m_outputAnimationData = null;

        public uint Duration => m_duration;
        public int BonesCount => m_bonesCount;

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

                string l_header = new string(l_reader.ReadChars(3));
                if(l_header != "RAF")
                    throw new Exception("Not ROC animation file");

                m_bonesCount = l_reader.ReadByte();
                m_duration = l_reader.ReadUInt32();

                m_boneAnimationData = new BoneAnimation[m_bonesCount];
                for(uint i = 0; i < m_bonesCount; i++)
                {
                    m_boneAnimationData[i] = new BoneAnimation();
                    m_boneAnimationData[i].m_positionsTimeline = new Timeline<vec3>();
                    m_boneAnimationData[i].m_rotationsTimeline = new Timeline<quat>();
                    m_boneAnimationData[i].m_scalesTimeline = new Timeline<vec3>();

                    int l_framekeys = l_reader.ReadInt32();
                    for(int j = 0; j < l_framekeys; j++)
                    {
                        uint l_time = l_reader.ReadUInt32();
                        vec3 l_pos = new vec3(l_reader.ReadSingle(), l_reader.ReadSingle(), l_reader.ReadSingle());
                        m_boneAnimationData[i].m_positionsTimeline.Add(l_time, l_pos);
                    }

                    l_framekeys = l_reader.ReadInt32();
                    for(int j = 0; j < l_framekeys; j++)
                    {
                        uint l_time = l_reader.ReadUInt32();
                        quat l_rot = new quat(l_reader.ReadSingle(), l_reader.ReadSingle(), l_reader.ReadSingle(), l_reader.ReadSingle());
                        m_boneAnimationData[i].m_rotationsTimeline.Add(l_time, l_rot);
                    }

                    l_framekeys = l_reader.ReadInt32();
                    for(int j = 0; j < l_framekeys; j++)
                    {
                        uint l_time = l_reader.ReadUInt32();
                        vec3 l_scl = new vec3(l_reader.ReadSingle(), l_reader.ReadSingle(), l_reader.ReadSingle());
                        m_boneAnimationData[i].m_scalesTimeline.Add(l_time, l_scl);
                    }
                }

                m_outputAnimationData = new AnimationFrame[m_bonesCount];
                m_loaded = true;
            }
            catch(Exception e)
            {
                m_log = e.Message;

                m_boneAnimationData = null;
                m_bonesCount = 0;
                m_duration = 0U;
            }
        }

        protected override void DestroyInternal()
        {
            if(m_loaded)
            {
                m_boneAnimationData = null;
                m_bonesCount = 0;
                m_duration = 0U;

                m_loaded = false;
            }

            base.DestroyInternal();
        }

        // Animation
        internal ref AnimationFrame[] GetFrameData(uint p_time)
        {
            if(!m_loaded)
                return ref ms_emptyAnimationFrame;

            p_time %= m_duration;

            for(int i = 0; i < m_bonesCount; i++)
            {
                m_outputAnimationData[i].m_hasPosition = false;
                m_outputAnimationData[i].m_hasRotation = false;
                m_outputAnimationData[i].m_hasScale = false;

                if(m_boneAnimationData[i].m_positionsTimeline.Get(p_time, out vec3 l_posA, out vec3 l_posB, out float l_blendA))
                {
                    m_outputAnimationData[i].m_hasPosition = true;
                    m_outputAnimationData[i].m_position = vec3.Lerp(l_posA, l_posB, l_blendA);
                }

                if(m_boneAnimationData[i].m_rotationsTimeline.Get(p_time, out quat l_quatA, out quat l_quatB, out float l_blendB))
                {
                    m_outputAnimationData[i].m_hasRotation = true;
                    m_outputAnimationData[i].m_rotation = Utils.SLerpSafe(l_quatA, l_quatB, l_blendB);
                }

                if(m_boneAnimationData[i].m_scalesTimeline.Get(p_time, out vec3 l_sclA, out vec3 l_sclB, out float l_blendC))
                {
                    m_outputAnimationData[i].m_hasScale = true;
                    m_outputAnimationData[i].m_scale = vec3.Lerp(l_sclA, l_sclB, l_blendC);
                }
            }

            return ref m_outputAnimationData;
        }

        // API
        public static Animation Import(string p_path)
        {
            Animation l_anim = new Animation();
            l_anim.Load(p_path);
            return l_anim;
        }
    }
}

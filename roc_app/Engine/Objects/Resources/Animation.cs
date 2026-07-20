using GlmSharp;
using glTFLoader;
using System;
using System.Collections.Generic;
using System.IO;

namespace ROC.Engine.Objects.Resources
{
    public sealed class Animation : Resource
    {
        public enum AnimationFileType : int
        {
            RAF = 0,
            glTF
        }

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

                uint l_fps = l_reader.ReadUInt32();
                uint l_framesCount = l_reader.ReadUInt32();
                m_bonesCount = (int)l_reader.ReadUInt32();

                uint l_frameDelta = 1000U / l_fps;
                m_duration = l_framesCount * l_frameDelta;

                m_boneAnimationData = new BoneAnimation[m_bonesCount];
                m_outputAnimationData = new AnimationFrame[m_bonesCount];
                for(uint i = 0; i < m_bonesCount; i++)
                {
                    m_boneAnimationData[i] = new BoneAnimation();
                    m_boneAnimationData[i].m_positionsTimeline = new Timeline<vec3>();
                    m_boneAnimationData[i].m_rotationsTimeline = new Timeline<quat>();
                    m_boneAnimationData[i].m_scalesTimeline = new Timeline<vec3>();

                    uint l_keyframesCount = l_reader.ReadUInt32();
                    for(uint j = 0; j < l_keyframesCount; j++)
                    {
                        vec3 l_pos = new vec3(l_reader.ReadSingle(), l_reader.ReadSingle(), l_reader.ReadSingle());
                        quat l_rot = new quat(l_reader.ReadSingle(), l_reader.ReadSingle(), l_reader.ReadSingle(), l_reader.ReadSingle());
                        vec3 l_scl = new vec3(l_reader.ReadSingle(), l_reader.ReadSingle(), l_reader.ReadSingle());
                        uint l_frameIndex = l_reader.ReadUInt32();
                        uint l_frameTime = l_frameIndex * l_frameDelta;
                        m_duration = Math.Max(m_duration, l_frameTime);

                        m_boneAnimationData[i].m_positionsTimeline.Add(l_frameTime, l_pos);
                        m_boneAnimationData[i].m_rotationsTimeline.Add(l_frameTime, l_rot.NormalizedSafe);
                        m_boneAnimationData[i].m_scalesTimeline.Add(l_frameTime, l_scl);
                    }
                }

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

        internal void Load(glTFLoader.Schema.Gltf p_model, int p_index, List<byte[]> p_buffers)
        {
            if(m_loaded)
                return;

            // Get list of nodes as bones
            List<glTFLoader.Schema.Node> l_bonesNodes = new List<glTFLoader.Schema.Node>();
            foreach(var l_joint in p_model.Skins[0].Joints)
                l_bonesNodes.Add(p_model.Nodes[l_joint]);

            m_bonesCount = l_bonesNodes.Count;
            m_boneAnimationData = new BoneAnimation[m_bonesCount];
            for(int i = 0; i < m_bonesCount; i++)
            {
                m_boneAnimationData[i].m_positionsTimeline = new Timeline<vec3>();
                m_boneAnimationData[i].m_rotationsTimeline = new Timeline<quat>();
                m_boneAnimationData[i].m_scalesTimeline = new Timeline<vec3>();
            }

            var l_anim = p_model.Animations[p_index];
            foreach(var l_channel in l_anim.Channels)
            {
                if(!l_channel.Target.Node.HasValue)
                    continue;

                int l_boneIndex = l_bonesNodes.IndexOf(p_model.Nodes[l_channel.Target.Node.Value]);
                if(l_boneIndex == -1)
                    continue;

                var l_times = Utils.GetAccessorData<float>(p_model, p_model.Accessors[l_anim.Samplers[l_channel.Sampler].Input], p_buffers); // N * 2 floats as time in seconds, where N is frames
                var l_values = Utils.GetAccessorData<float>(p_model, p_model.Accessors[l_anim.Samplers[l_channel.Sampler].Output], p_buffers); // 6 * N * 2 floats for position and scale, 8 * N * 2 floats for rotation
                int l_frames = l_times.Length / 2;
                switch(l_channel.Target.Path)
                {
                    case glTFLoader.Schema.AnimationChannelTarget.PathEnum.translation:
                    {
                        for(int i = 0; i < l_frames; i++)
                        {
                            m_boneAnimationData[l_boneIndex].m_positionsTimeline.Add((uint)(l_times[i * 2] * 1000f), new vec3(l_values[i * 6], l_values[i * 6 + 1], l_values[i * 6 + 2]));
                            m_boneAnimationData[l_boneIndex].m_positionsTimeline.Add((uint)(l_times[i * 2 + 1] * 1000f), new vec3(l_values[i * 6 + 3], l_values[i * 6 + 4], l_values[i * 6 + 5]));
                        }
                    }
                    break;

                    case glTFLoader.Schema.AnimationChannelTarget.PathEnum.rotation:
                    {
                        for(int i = 0; i < l_frames; i++)
                        {
                            m_boneAnimationData[l_boneIndex].m_rotationsTimeline.Add((uint)(l_times[i * 2] * 1000f), new quat(l_values[i * 8], l_values[i * 8 + 1], l_values[i * 8 + 2], l_values[i * 8 + 3]));
                            m_boneAnimationData[l_boneIndex].m_rotationsTimeline.Add((uint)(l_times[i * 2 + 1] * 1000f), new quat(l_values[i * 8 + 4], l_values[i * 8 + 5], l_values[i * 8 + 6], l_values[i * 8 + 7]).NormalizedSafe);
                        }
                    }
                    break;

                    case glTFLoader.Schema.AnimationChannelTarget.PathEnum.scale:
                    {
                        for(int i = 0; i < l_frames; i++)
                        {
                            m_boneAnimationData[l_boneIndex].m_scalesTimeline.Add((uint)(l_times[i * 2] * 1000f), new vec3(l_values[i * 6], l_values[i * 6 + 1], l_values[i * 6 + 2]));
                            m_boneAnimationData[l_boneIndex].m_scalesTimeline.Add((uint)(l_times[i * 2 + 1] * 1000f), new vec3(l_values[i * 6 + 3], l_values[i * 6 + 4], l_values[i * 6 + 5]));
                        }
                    }
                    break;
                }

                m_duration = Math.Max(m_duration, (uint)(p_model.Accessors[l_anim.Samplers[l_channel.Sampler].Input].Max[0] * 1000f));
            }

            m_outputAnimationData = new AnimationFrame[m_bonesCount];
            m_loaded = true;
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
        public static Animation ImportRAF(string p_path)
        {
            Animation l_anim = new Animation();
            l_anim.Load(p_path);
            return l_anim;
        }

        public static Dictionary<string, Animation> ImportGLTF(string p_path)
        {
            var l_dict = new Dictionary<string, Animation>();

            try
            {
                var l_gltfModel = Interface.LoadModel(p_path);

                List<byte[]> l_binaryBuffers = new List<byte[]>();
                foreach(var l_buffer in l_gltfModel.Buffers)
                {
                    if(l_buffer.Uri != null)
                        l_binaryBuffers.Add(Convert.FromBase64String(l_buffer.Uri.Split(',')[1]));
                    else
                        l_binaryBuffers.Add(Interface.LoadBinaryBuffer(l_gltfModel, l_binaryBuffers.Count, p_path));
                }

                if(l_gltfModel.Skins == null || l_gltfModel.Skins.Length == 0 || l_gltfModel.Skins[0].Joints == null || l_gltfModel.Skins[0].Joints.Length == 0)
                    throw new Exception(string.Format("'{0}' has no skin or joints data", p_path));

                if(l_gltfModel.Animations == null || l_gltfModel.Animations.Length == 0)
                    throw new Exception(string.Format("'{0}' has no animation data", p_path));

                for(int i = 0, j = l_gltfModel.Animations.Length; i < j; i++)
                {
                    Animation l_anim = new Animation();
                    l_anim.Load(l_gltfModel, i, l_binaryBuffers);
                    l_dict.Add(l_gltfModel.Animations[i].Name ?? string.Format("Animation{0}", i), l_anim);
                }
            }
            catch(Exception e)
            {
                Console.WriteLine(e.Message);
            }

            return l_dict;
        }
    }
}

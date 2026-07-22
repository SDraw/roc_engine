using glTFLoader.Schema;
using System;
using System.Collections.Generic;
using System.IO;

namespace ROC.Converter
{
    internal class GLTFAnimation
    {
        class BoneAnimationData
        {
            public float[] m_positionTimes;
            public float[] m_positionValues;

            public float[] m_rotationTimes;
            public float[] m_rotationValues;

            public float[] m_scaleTimes;
            public float[] m_scaleValues;
        }

        bool m_loaded = false;
        int m_bonesCount = 0;
        uint m_duration = 0;
        string m_name = "";

        readonly List<BoneAnimationData> m_bonesData = new List<BoneAnimationData>();

        public GLTFAnimation()
        {
        }

        public void Parse(Gltf p_model, int p_index, List<byte[]> p_buffers)
        {
            if(m_loaded)
                return;

            try
            {
                List<Node> l_bonesNodes = new List<Node>();
                foreach(var l_joint in p_model.Skins[0].Joints)
                    l_bonesNodes.Add(p_model.Nodes[l_joint]);

                m_bonesCount = l_bonesNodes.Count;
                for(int i = 0; i < m_bonesCount; i++)
                    m_bonesData.Add(new BoneAnimationData());

                var l_anim = p_model.Animations[p_index];
                m_name = l_anim.Name ?? ("Animation" + p_index);
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
                        case AnimationChannelTarget.PathEnum.translation:
                        {
                            m_bonesData[l_boneIndex].m_positionTimes = l_times;
                            m_bonesData[l_boneIndex].m_positionValues = l_values;
                        }
                        break;

                        case AnimationChannelTarget.PathEnum.rotation:
                        {
                            for(int i = 0; i < l_frames; i++)
                            {
                                m_bonesData[l_boneIndex].m_rotationTimes = l_times;
                                m_bonesData[l_boneIndex].m_rotationValues = l_values;
                            }
                        }
                        break;

                        case AnimationChannelTarget.PathEnum.scale:
                        {
                            m_bonesData[l_boneIndex].m_scaleTimes = l_times;
                            m_bonesData[l_boneIndex].m_scaleValues = l_values;
                        }
                        break;
                    }

                    m_duration = Math.Max(m_duration, (uint)(p_model.Accessors[l_anim.Samplers[l_channel.Sampler].Input].Max[0] * 1000f));
                }

                m_loaded = true;

                Console.WriteLine("Loaded animation '{0}' from index {1}", m_name, p_index);
            }
            catch(Exception e)
            {
                Console.WriteLine("Error parsing animation with index {0}: {1} at {2}", p_index, e.Message, e.StackTrace);
            }
        }

        public void Save(string p_dir)
        {
            if(!m_loaded)
                return;

            try
            {
                string l_dir = Path.Combine(p_dir, "animations");
                if(!Directory.Exists(l_dir))
                    Directory.CreateDirectory(l_dir);

                FileStream l_file = new FileStream(Path.Combine(l_dir, m_name + ".raf"), FileMode.Create, FileAccess.ReadWrite);
                BinaryWriter l_writer = new BinaryWriter(l_file);

                l_writer.Write((byte)'R');
                l_writer.Write((byte)'A');
                l_writer.Write((byte)'F');

                l_writer.Write(m_duration);
                l_writer.Write((byte)m_bonesCount);

                for(int i = 0; i < m_bonesCount; i++)
                {
                    int l_framekeys = m_bonesData[i].m_positionTimes.Length;
                    l_writer.Write(l_framekeys);
                    for(int j = 0; j < l_framekeys; j++)
                    {
                        l_writer.Write((uint)(m_bonesData[i].m_positionTimes[j] * 1000f));
                        l_writer.Write(m_bonesData[i].m_positionValues[j * 3]);
                        l_writer.Write(m_bonesData[i].m_positionValues[j * 3 + 1]);
                        l_writer.Write(m_bonesData[i].m_positionValues[j * 3 + 2]);
                    }

                    l_framekeys = m_bonesData[i].m_rotationTimes.Length;
                    l_writer.Write(l_framekeys);
                    for(int j = 0; j < l_framekeys; j++)
                    {
                        l_writer.Write((uint)(m_bonesData[i].m_rotationTimes[j] * 1000f));
                        l_writer.Write(m_bonesData[i].m_rotationValues[j * 4]);
                        l_writer.Write(m_bonesData[i].m_rotationValues[j * 4 + 1]);
                        l_writer.Write(m_bonesData[i].m_rotationValues[j * 4 + 2]);
                        l_writer.Write(m_bonesData[i].m_rotationValues[j * 4 + 3]);
                    }

                    l_framekeys = m_bonesData[i].m_scaleTimes.Length;
                    l_writer.Write(l_framekeys);
                    for(int j = 0; j < l_framekeys; j++)
                    {
                        l_writer.Write((uint)(m_bonesData[i].m_scaleTimes[j] * 1000f));
                        l_writer.Write(m_bonesData[i].m_scaleValues[j * 3]);
                        l_writer.Write(m_bonesData[i].m_scaleValues[j * 3 + 1]);
                        l_writer.Write(m_bonesData[i].m_scaleValues[j * 3 + 2]);
                    }
                }

                l_writer.Flush();
                l_writer.Close();
                l_writer.Dispose();

                l_file.Close();
                l_file.Dispose();

                Console.WriteLine("Animation '{0}' saved to {1}", m_name, Path.Combine(l_dir, m_name + ".raf"));
            }
            catch(Exception e)
            {
                Console.WriteLine("Error saving animation '{0}': {1} at {2}", m_name, e.Message, e.StackTrace);
            }
        }
    }
}

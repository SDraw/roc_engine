using SFML.Audio;
using System;

namespace ROC.Engine.Objects.Resources
{
    public sealed class AudioClip : Resource
    {
        SoundBuffer m_buffer = null;

        internal Action OnDestroy;

        internal AudioClip()
        {
        }

        internal void Load(string p_path)
        {
            if(m_loaded)
                return;

            try
            {
                m_buffer = new SoundBuffer(p_path);
                m_loaded = true;
            }
            catch(Exception e)
            {
                m_buffer = null;
                m_log = e.Message;
            }
        }

        public override void Unload()
        {
            if(!m_loaded)
                return;

            OnDestroy?.Invoke();

            m_buffer.Dispose();
            m_buffer = null;
            m_loaded = false;
        }

        internal SoundBuffer Buffer => m_buffer;

        public uint SampleRate => (m_buffer != null) ? m_buffer.SampleRate : 0U;

        public uint Channels => (m_buffer != null) ? m_buffer.ChannelCount : 0U;

        public float Duration => (m_buffer != null) ? m_buffer.Duration.AsSeconds() : 0f;

        // API
        public static AudioClip Import(string p_path)
        {
            AudioClip l_clip = new AudioClip();
            l_clip.Load(p_path);
            return l_clip;
        }
    }
}

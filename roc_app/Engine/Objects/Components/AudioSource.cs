using ROC.Engine.Objects.Resources;
using SFML.Audio;
using SFML.System;
using System;

namespace ROC.Engine.Objects.Components
{
    public sealed class AudioSource : Component
    {
        readonly Sound m_sound = null;
        AudioClip m_clip = null;

        internal AudioSource()
        {
            m_componentType = ComponentType.AudioSource;

            m_sound = new Sound();
            PlayOnEnable = false;
        }

        public AudioClip AudioClip
        {
            get => m_clip;
            set
            {
                if(m_clip != null)
                {
                    m_clip.OnDestroy -= this.AudioClipCleanup;
                    AudioClipCleanup();
                }

                m_clip = value;
                if((m_clip != null) && m_clip.IsLoaded)
                {
                    m_clip.OnDestroy += this.AudioClipCleanup;
                    m_sound.SoundBuffer = m_clip.Buffer;
                }
                else
                    m_clip = null;
            }
        }

        void AudioClipCleanup()
        {
            m_clip.OnDestroy -= this.AudioClipCleanup;
            m_clip = null;

            m_sound.Stop();
            m_sound.SoundBuffer = null;
        }

        public float Time
        {
            get => m_sound.PlayingOffset.AsSeconds();
            set => m_sound.PlayingOffset = SFML.System.Time.FromSeconds(Math.Max(0f, value));
        }

        public float Volume
        {
            get => m_sound.Volume;
            set => m_sound.Volume = Math.Min(Math.Max(0f, value), 100f);
        }

        public float Pitch
        {
            get => m_sound.Pitch;
            set => m_sound.Pitch = value;
        }

        public bool Loop
        {
            get => m_sound.Loop;
            set => m_sound.Loop = value;
        }

        public float MinDistance
        {
            get => m_sound.MinDistance;
            set => m_sound.MinDistance = Math.Max(0.1f, value);
        }

        public float Attenuation
        {
            get => m_sound.Attenuation;
            set => m_sound.Attenuation = Math.Max(0f, value);
        }

        public SoundStatus Status => m_sound.Status;

        public bool PlayOnEnable
        {
            get;
            set;
        }

        public void Play()
        {
            if(m_clip == null)
                return;

            m_sound.Play();
        }

        public void Pause()
        {
            if(m_clip == null)
                return;

            m_sound.Pause();
        }

        public void Stop()
        {
            if(m_clip == null)
                return;

            m_sound.Stop();
        }

        // Component
        internal override void OnRenderPass(Shader p_shader, Camera p_camera)
        {
            var l_pos = GameObject.Position;
            m_sound.Position = new Vector3f(l_pos.x, l_pos.y, l_pos.z);
        }

        internal override void OnDisable() => Stop();
        internal override void OnEnable()
        {
            if(PlayOnEnable)
                Play();
        }
    }
}

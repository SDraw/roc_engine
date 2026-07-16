namespace ROC.Engine.Managers
{
    public sealed class SoundManager : Manager
    {
        float m_globalVolume;

        internal SoundManager()
        {
            m_globalVolume = 100f;
        }

        public float GlobalVolume
        {
            get => m_globalVolume;
            set
            {
                m_globalVolume = value;
                SFML.Audio.Listener.GlobalVolume = m_globalVolume;
            }
        }
    }
}

using GlmSharp;

namespace ROC.Module.Wrappers
{
    public sealed class Quaternion
    {
        public quat m_quat;

        public Quaternion()
        {
            m_quat = quat.Identity;
        }

        public Quaternion(float x, float y, float z, float w)
        {
            m_quat = new quat(x, y, z, w);
        }

        public Quaternion(quat quat)
        {
            m_quat = quat;
        }

        public Quaternion(Quaternion quat)
        {
            m_quat = quat.m_quat;
        }
    }
}

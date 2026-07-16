using GlmSharp;

namespace ROC.Module.Wrappers
{
    public sealed class Vector3
    {
        public vec3 m_vector;

        public Vector3()
        {
            m_vector = vec3.Zero;
        }

        public Vector3(float x, float y, float z)
        {
            m_vector = new vec3(x, y, z);
        }

        public Vector3(vec3 vec)
        {
            m_vector = vec;
        }

        public Vector3(Vector3 vec)
        {
            m_vector = vec.m_vector;
        }
    }
}

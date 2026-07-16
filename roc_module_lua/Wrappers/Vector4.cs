using GlmSharp;

namespace ROC.Module.Wrappers
{
    public sealed class Vector4
    {
        public vec4 m_vector;

        public Vector4()
        {
            m_vector = vec4.Zero;
        }

        public Vector4(float x, float y, float z, float w)
        {
            m_vector = new vec4(x, y, z, w);
        }

        public Vector4(vec4 vec)
        {
            m_vector = vec;
        }

        public Vector4(Vector4 vec)
        {
            m_vector = vec.m_vector;
        }
    }
}

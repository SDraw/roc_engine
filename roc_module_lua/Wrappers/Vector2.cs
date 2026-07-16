using GlmSharp;

namespace ROC.Module.Wrappers
{
    public sealed class Vector2
    {
        public vec2 m_vector;

        public Vector2()
        {
            m_vector = vec2.Zero;
        }

        public Vector2(float x, float y)
        {
            m_vector = new vec2(x, y);
        }

        public Vector2(vec2 vec)
        {
            m_vector = vec;
        }

        public Vector2(Vector2 vec)
        {
            m_vector = vec.m_vector;
        }
    }
}

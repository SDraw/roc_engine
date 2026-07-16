using GlmSharp;

namespace ROC.Module.Wrappers
{
    public sealed class Matrix4
    {
        public mat4 m_matrix;

        public Matrix4()
        {
            m_matrix = mat4.Identity;
        }

        public Matrix4(mat4 mat)
        {
            m_matrix = mat;
        }

        public Matrix4(Matrix4 mat)
        {
            m_matrix = mat.m_matrix;
        }
    }
}

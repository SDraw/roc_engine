using GlmSharp;

namespace ROC.Module.Wrappers
{
    public sealed class Matrix3
    {
        public mat3 m_matrix;

        public Matrix3()
        {
            m_matrix = mat3.Identity;
        }

        public Matrix3(mat3 mat)
        {
            m_matrix = mat;
        }

        public Matrix3(Matrix3 mat)
        {
            m_matrix = mat.m_matrix;
        }
    }
}

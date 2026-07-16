using GlmSharp;

namespace ROC.Module.Wrappers
{
    public sealed class Matrix2
    {
        public mat2 m_matrix;

        public Matrix2()
        {
            m_matrix = mat2.Identity;
        }

        public Matrix2(mat2 mat)
        {
            m_matrix = mat;
        }

        public Matrix2(Matrix2 mat)
        {
            m_matrix = mat.m_matrix;
        }
    }
}

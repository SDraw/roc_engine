using OpenGL;
using System;

namespace ROC.Engine.OGL
{
    internal sealed class GLVertexArray
    {
        [ThreadStatic]
        static uint ms_activeID = 0U;

        uint m_internalID = 0U;

        public GLVertexArray()
        {
            m_internalID = 0U;
        }

        public bool IsValid => (m_internalID != 0U);

        public void Create()
        {
            if(m_internalID != 0U)
                return;

            m_internalID = Gl.GenVertexArray();
        }

        public void Destroy()
        {
            if(m_internalID == 0U)
                return;

            if(ms_activeID == m_internalID)
                ms_activeID = 0U;

            Gl.DeleteVertexArrays(m_internalID);
            m_internalID = 0U;
        }

        public void EnableAttribute(uint p_index, int p_size, int p_type)
        {
            if(m_internalID == 0U)
                return;

            Bind();
            Gl.EnableVertexAttribArray(p_index);

            switch(p_type)
            {
                case Gl.FLOAT:
                    Gl.VertexAttribPointer(p_index, p_size, VertexAttribType.Float, false, 0, IntPtr.Zero);
                    break;

                case Gl.INT:
                    Gl.VertexAttribIPointer(p_index, p_size, VertexAttribType.Int, 0, IntPtr.Zero);
                    break;
            }
        }

        public void Bind()
        {
            if(m_internalID == 0U)
                return;

            if(ms_activeID == m_internalID)
                return;

            Gl.BindVertexArray(m_internalID);
            ms_activeID = m_internalID;
        }

        public void Draw(PrimitiveType p_mode, int p_count)
        {
            if(m_internalID == 0U)
                return;

            Bind();
            Gl.DrawArrays(p_mode, 0, p_count);
        }
    }
}

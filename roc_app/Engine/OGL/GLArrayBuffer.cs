using OpenGL;
using System;

namespace ROC.Engine.OGL
{
    internal sealed class GLArrayBuffer
    {
        [ThreadStatic]
        static uint ms_activeID = 0U;

        uint m_internalID = 0U;

        public bool IsValid => (m_internalID != 0U);

        public GLArrayBuffer()
        {
            m_internalID = 0U;
        }

        public void Create(int p_size, object p_data, BufferUsage p_usage)
        {
            if(m_internalID != 0U)
                return;

            m_internalID = Gl.GenBuffer();

            if(m_internalID == 0U)
                return;

            Gl.BindBuffer(BufferTarget.ArrayBuffer, m_internalID);
            Gl.BufferData(BufferTarget.ArrayBuffer, (uint)p_size, p_data, p_usage);

            if(ms_activeID != 0U)
                Gl.BindBuffer(BufferTarget.ArrayBuffer, ms_activeID);
        }

        public void Destroy()
        {
            if(m_internalID == 0U)
                return;

            if(ms_activeID == m_internalID)
                ms_activeID = 0U;

            Gl.DeleteBuffers(m_internalID);
            m_internalID = 0U;
        }

        public void Bind()
        {
            if(m_internalID == 0U)
                return;

            if(ms_activeID == m_internalID)
                return;

            Gl.BindBuffer(BufferTarget.ArrayBuffer, m_internalID);
            ms_activeID = m_internalID;
        }


    }
}

using OpenGL;
using System;

namespace ROC.Engine.OGL
{
    internal sealed class GLRenderBuffer
    {
        [ThreadStatic]
        static uint ms_activeID = 0U;

        uint m_internalID = 0U;

        public bool IsValid => (m_internalID != 0U);
        internal uint InternalD => m_internalID;

        public GLRenderBuffer()
        {
        }

        public void Create(InternalFormat p_format, int p_width, int p_height)
        {
            if(m_internalID != 0U)
                return;

            m_internalID = Gl.GenRenderbuffer();
            if(m_internalID == 0U)
                return;

            Gl.BindRenderbuffer(RenderbufferTarget.Renderbuffer, m_internalID);
            Gl.RenderbufferStorage(RenderbufferTarget.Renderbuffer, p_format, p_width, p_height);
            Gl.BindRenderbuffer(RenderbufferTarget.Renderbuffer, ms_activeID);
        }

        public void Destroy()
        {
            if(m_internalID == 0U)
                return;

            Gl.DeleteRenderbuffers(m_internalID);

            if(ms_activeID == m_internalID)
            {
                Gl.BindRenderbuffer(RenderbufferTarget.Renderbuffer, 0U);
                m_internalID = 0U;
            }
        }

        public void Bind()
        {
            if(m_internalID == 0U)
                return;

            if(ms_activeID == m_internalID)
                return;

            Gl.BindRenderbuffer(RenderbufferTarget.Renderbuffer, m_internalID);
            ms_activeID = m_internalID;
        }

        public static void Reset()
        {
            if(ms_activeID == 0U)
                return;

            Gl.BindRenderbuffer(RenderbufferTarget.Renderbuffer, 0U);
            ms_activeID = 0U;
        }
    }
}

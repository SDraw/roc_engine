using OpenGL;
using System;

namespace ROC.Engine.OGL
{
    internal sealed class GLFrameBuffer
    {
        [ThreadStatic]
        static uint ms_activeID = 0U;

        uint m_internalID = 0U;

        public bool IsValid => (m_internalID != 0U);

        public GLFrameBuffer()
        {
        }

        public void Create()
        {
            if(m_internalID != 0U)
                return;

            m_internalID = Gl.GenFramebuffer();
        }

        public void Destroy()
        {
            if(m_internalID == 0U)
                return;

            Gl.DeleteFramebuffers(m_internalID);

            if(ms_activeID == m_internalID)
            {
                Gl.BindFramebuffer(FramebufferTarget.Framebuffer, 0U);
                ms_activeID = 0U;
            }

            m_internalID = 0U;
        }

        // Attachments
        public void SetRenderBuffer(FramebufferAttachment p_attachment, uint p_id)
        {
            if(m_internalID == 0U)
                return;

            if(ms_activeID != m_internalID)
                Gl.BindFramebuffer(FramebufferTarget.Framebuffer, m_internalID);

            Gl.FramebufferRenderbuffer(FramebufferTarget.Framebuffer, p_attachment, RenderbufferTarget.Renderbuffer, p_id);

            if(ms_activeID != m_internalID)
                Gl.BindFramebuffer(FramebufferTarget.Framebuffer, ms_activeID);
        }

        public void SetTexture(FramebufferAttachment p_attachment, uint p_id)
        {
            if(m_internalID == 0U)
                return;

            if(ms_activeID != m_internalID)
                Gl.BindFramebuffer(FramebufferTarget.Framebuffer, m_internalID);

            Gl.FramebufferTexture2D(FramebufferTarget.Framebuffer, p_attachment, TextureTarget.Texture2d, p_id, 0);

            if(ms_activeID != m_internalID)
                Gl.BindFramebuffer(FramebufferTarget.Framebuffer, ms_activeID);
        }

        public void SetDrawBuffer(DrawBufferMode p_mode)
        {
            if(m_internalID == 0U)
                return;

            if(ms_activeID != m_internalID)
                Gl.BindFramebuffer(FramebufferTarget.Framebuffer, m_internalID);

            Gl.DrawBuffer(p_mode);

            if(ms_activeID != m_internalID)
                Gl.BindFramebuffer(FramebufferTarget.Framebuffer, ms_activeID);
        }

        public void SetReadBuffer(ReadBufferMode p_mode)
        {
            if(m_internalID == 0U)
                return;

            if(ms_activeID != m_internalID)
                Gl.BindFramebuffer(FramebufferTarget.Framebuffer, m_internalID);

            Gl.ReadBuffer(p_mode);

            if(ms_activeID != m_internalID)
                Gl.BindFramebuffer(FramebufferTarget.Framebuffer, ms_activeID);
        }

        // Binding
        public void Bind()
        {
            if(m_internalID == 0U)
                return;

            if(ms_activeID == m_internalID)
                return;

            Gl.BindFramebuffer(FramebufferTarget.Framebuffer, m_internalID);
            ms_activeID = m_internalID;
        }

        public static void Reset()
        {
            if(ms_activeID == 0U)
                return;

            Gl.BindFramebuffer(FramebufferTarget.Framebuffer, 0U);
            ms_activeID = 0U;
        }
    }
}

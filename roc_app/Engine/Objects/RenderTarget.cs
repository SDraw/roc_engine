using GlmSharp;
using OpenGL;
using ROC.Engine.OGL;

namespace ROC.Engine.Objects
{
    internal sealed class RenderTarget : Object
    {
        public enum RenderTargetType
        {
            Shadow = 0,
            RGB,
            RGBA,
            RGBF,
            RGBAF,

            Invalid = -1,
        }

        RenderTargetType m_type = RenderTargetType.Invalid;
        public RenderTargetType TypeOfRenderTarget => m_type;

        ivec2 m_size = ivec2.Zero;

        GLFrameBuffer m_glFrameBuffer = null;
        GLTexture2D m_glTexture = null;
        GLRenderBuffer m_glRenderBuffer = null;

        public override bool IsValid => (m_type != RenderTargetType.Invalid);

        public RenderTarget()
        {
        }

        public void Create(RenderTargetType p_type, ivec2 p_size, int p_filter = Gl.LINEAR)
        {
            if(m_type != RenderTargetType.Invalid || p_type == RenderTargetType.Invalid)
                return;

            m_type = p_type;
            m_size = p_size;

            m_glFrameBuffer = new GLFrameBuffer();
            m_glFrameBuffer.Create();
            m_glFrameBuffer.Bind();

            m_glTexture = new GLTexture2D();
            switch(m_type)
            {
                case RenderTargetType.Shadow:
                {
                    m_glTexture.Create(m_size.x, m_size.y, InternalFormat.DepthComponent32, PixelFormat.DepthComponent, null, p_filter);
                    m_glTexture.SetCompareFunction(Gl.LEQUAL);
                    m_glTexture.SetCompareMode(Gl.COMPARE_REF_DEPTH_TO_TEXTURE_EXT);
                    m_glTexture.SetWrap(Gl.CLAMP_TO_EDGE);
                }
                break;

                case RenderTargetType.RGB:
                    m_glTexture.Create(m_size.x, m_size.y, InternalFormat.Rgb8, PixelFormat.Rgb, null, p_filter);
                    break;

                case RenderTargetType.RGBA:
                    m_glTexture.Create(m_size.x, m_size.y, InternalFormat.Rgba8, PixelFormat.Rgba, null, p_filter);
                    break;

                case RenderTargetType.RGBF:
                    m_glTexture.Create(m_size.x, m_size.y, InternalFormat.Rgb16f, PixelFormat.Rgb, null, p_filter);
                    break;

                case RenderTargetType.RGBAF:
                    m_glTexture.Create(m_size.x, m_size.y, InternalFormat.Rgba32f, PixelFormat.Rgba, null, p_filter);
                    break;
            }

            if(m_type > RenderTargetType.Shadow)
            {
                m_glRenderBuffer = new GLRenderBuffer();
                m_glRenderBuffer.Create(InternalFormat.DepthComponent, m_size.x, m_size.y);
                m_glRenderBuffer.Bind();

                m_glFrameBuffer.SetRenderBuffer(FramebufferAttachment.DepthAttachment, m_glRenderBuffer.InternalD);
                m_glFrameBuffer.SetTexture(FramebufferAttachment.ColorAttachment0, m_glTexture.InternalID);
                m_glFrameBuffer.SetDrawBuffer((DrawBufferMode)Gl.COLOR_ATTACHMENT0);

                GLRenderBuffer.Reset();
            }
            else
            {
                m_glFrameBuffer.SetTexture(FramebufferAttachment.DepthAttachment, m_glTexture.InternalID);
                m_glFrameBuffer.SetDrawBuffer(Gl.NONE);
            }
            m_glFrameBuffer.SetReadBuffer(Gl.NONE);

            GLFrameBuffer.Reset();
        }

        protected override void DestroyInternal()
        {
            if(m_type != RenderTargetType.Invalid)
            {
                m_glTexture?.Destroy();
                m_glTexture = null;

                m_glRenderBuffer?.Destroy();
                m_glRenderBuffer = null;

                m_glFrameBuffer?.Destroy();
                m_glFrameBuffer = null;

                m_type = RenderTargetType.Invalid;
                m_size = ivec2.Zero;
            }

            base.DestroyInternal();
        }

        public void Bind(bool p_clear = true)
        {
            if(m_type == RenderTargetType.Invalid)
                return;

            m_glFrameBuffer.Bind();
            GLViewport.SetArea(ivec2.Zero, m_size);

            if(p_clear)
            {
                GLSettings.SetDepthMask(true);
                GLViewport.Clear(true, true);
            }
        }

        public void BindTexture(TextureUnit p_slot = TextureUnit.Texture1)
        {
            if(m_type == RenderTargetType.Invalid)
                return;

            m_glTexture.Bind(p_slot);
        }

        public static void Reset()
        {
            GLFrameBuffer.Reset();
            GLViewport.SetArea(ivec2.Zero, Managers.RenderManager.RenderSize);
        }
    }
}

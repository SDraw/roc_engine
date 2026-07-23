using GlmSharp;
using OpenGL;
using ROC.Engine.OGL;
using System;

namespace ROC.Engine.Objects.Resources
{
    public sealed class Texture : Resource
    {
        public enum TextureFiltering
        {
            Nearest = 0,
            Linear
        }

        static readonly byte[] ms_dummyTextureData = new byte[]
        {
            0x7F, 0x7F, 0x7F,
            0xF7, 0x94, 0x1D,
            0xF7, 0x94, 0x1D,
            0x7F, 0x7F, 0x7F
        };

        GLTexture2D m_glTexture = null;
        uvec2 m_size = uvec2.Zero;

        public uvec2 Size => m_size;

        internal Texture()
        {
        }

        internal void Load(string p_path, bool p_transparent, TextureFiltering p_filter)
        {
            if(m_loaded)
                return;

            m_glTexture = new GLTexture2D();

            try
            {
                SFML.Graphics.Image l_image = new SFML.Graphics.Image(p_path);

                m_glTexture.Create(
                    (int)l_image.Size.X, (int)l_image.Size.Y,
                    p_transparent ? InternalFormat.Rgba8 : InternalFormat.Rgb8,
                    PixelFormat.Rgba,
                    l_image.Pixels,
                    Gl.NEAREST + (int)p_filter
                );
            }
            catch(Exception e)
            {
                m_log = e.Message;
                m_glTexture.Create(2, 2, InternalFormat.Rgb8, PixelFormat.Rgb, ms_dummyTextureData, Gl.NEAREST);

            }

            m_loaded = true;
        }

        internal void Load(byte[] p_data, bool p_transparent, bool p_compressed, TextureFiltering p_filter)
        {
            if(m_loaded)
                return;

            m_glTexture = new GLTexture2D();

            try
            {
                SFML.Graphics.Image l_image = new SFML.Graphics.Image(p_data);

                m_glTexture.Create(
                    (int)l_image.Size.X, (int)l_image.Size.Y,
                    p_transparent ? InternalFormat.Rgba8 : InternalFormat.Rgb8,
                    PixelFormat.Rgba,
                    l_image.Pixels,
                    Gl.NEAREST + (int)p_filter
                );
                m_loaded = true;
            }
            catch(Exception)
            {
                m_glTexture.Create(2, 2, InternalFormat.CompressedRgb, PixelFormat.Rgba, ms_dummyTextureData, Gl.NEAREST);
            }
        }

        protected override void DestroyInternal()
        {
            if(m_loaded)
            {
                m_glTexture.Destroy();
                m_glTexture = null;
                m_size = uvec2.Zero;

                m_loaded = false;
            }

            base.DestroyInternal();
        }

        internal void Activate(TextureUnit p_slot)
        {
            if(!m_loaded)
                return;

            m_glTexture.Bind(p_slot);
        }

        // API
        public static Texture Import(string p_path, bool p_alpha = true, TextureFiltering p_filter = TextureFiltering.Nearest)
        {
            Texture l_texture = new Texture();
            l_texture.Load(p_path, p_alpha, p_filter);
            return l_texture;
        }
    }
}

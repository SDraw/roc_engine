using OpenGL;
using ROC.Engine.OGL;
using System;

namespace ROC.Engine.Objects.Resources
{
    public sealed class Texture : Resource
    {
        static readonly byte[] ms_dummyTextureData = new byte[]
        {
            0x7F, 0x7F, 0x7F,
            0xF7, 0x94, 0x1D,
            0xF7, 0x94, 0x1D,
            0x7F, 0x7F, 0x7F
        };

        GLTexture2D m_glTexture = null;

        internal Texture()
        {
        }

        internal void Load(string p_path, bool p_transparent, bool p_compressed, int p_filter)
        {
            if(m_loaded)
                return;

            m_glTexture = new GLTexture2D();

            try
            {
                SFML.Graphics.Image l_image = new SFML.Graphics.Image(p_path);

                m_glTexture.Create(
                    (int)l_image.Size.X, (int)l_image.Size.Y,
                    p_transparent ? (p_compressed ? InternalFormat.CompressedRgba : InternalFormat.Rgba8) : (p_compressed ? InternalFormat.CompressedRgb : InternalFormat.Rgb8),
                    PixelFormat.Rgba,
                    l_image.Pixels,
                    p_filter
                );
                m_loaded = true;
            }
            catch(Exception)
            {
                m_glTexture.Create(2, 2, InternalFormat.CompressedRgb, PixelFormat.Rgba, ms_dummyTextureData, Gl.NEAREST);
            }
        }

        public override void Unload()
        {
            if(!m_loaded)
                return;

            m_glTexture.Destroy();
            m_glTexture = null;

            m_loaded = false;
        }

        internal void Activate(TextureUnit p_slot)
        {
            if(!m_loaded)
                return;

            m_glTexture.Bind(p_slot);
        }

        // API
        public static Texture Import(string p_path, bool p_alpha = true, bool p_compress = false, int p_filter = 0)
        {
            Texture l_texture = new Texture();
            l_texture.Load(p_path, p_alpha, p_compress, p_filter);
            return l_texture;
        }
    }
}

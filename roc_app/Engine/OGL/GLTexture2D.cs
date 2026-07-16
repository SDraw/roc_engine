using OpenGL;

namespace ROC.Engine.OGL
{
    internal sealed class GLTexture2D : GLTexture
    {
        public void Create(int p_width, int p_height, InternalFormat p_format, PixelFormat p_pixelFormat, byte[] p_data, int p_filter = Gl.NEAREST)
        {
            if(m_internalID != 0U)
                return;

            m_internalType = TextureTarget.Texture2d;
            m_internalID = Gl.GenTexture();

            if(m_internalID == 0)
                return;

            Gl.ActiveTexture(TextureUnit.Texture0);
            Gl.BindTexture(m_internalType, m_internalID);
            Gl.TexParameterI(m_internalType, TextureParameterName.TextureWrapS, new int[1] { Gl.REPEAT });
            Gl.TexParameterI(m_internalType, TextureParameterName.TextureWrapT, new int[1] { Gl.REPEAT });
            Gl.TexParameterI(m_internalType, TextureParameterName.TextureMagFilter, new int[1] { p_filter });
            Gl.TexParameterI(m_internalType, TextureParameterName.TextureMinFilter, new int[1] { p_filter });
            Gl.TexImage2D(m_internalType, 0, p_format, p_width, p_height, 0, p_pixelFormat, PixelType.UnsignedByte, p_data);

            RebindSlots();
        }
    }
}

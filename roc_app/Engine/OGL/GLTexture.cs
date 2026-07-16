using OpenGL;
using System;

namespace ROC.Engine.OGL
{
    internal class GLTexture
    {
        const int c_maxTextureUnits = 32;

        [ThreadStatic]
        protected static uint[] ms_activeID = new uint[c_maxTextureUnits];

        [ThreadStatic]
        protected static TextureTarget[] ms_activeType = new TextureTarget[c_maxTextureUnits];

        protected uint m_internalID = 0;
        protected TextureTarget m_internalType;

        public bool IsValid => (m_internalID != 0U);
        internal uint InternalID => m_internalID;

        protected GLTexture()
        {
            m_internalID = 0;
        }

        public void Destroy()
        {
            if(m_internalID == 0U)
                return;

            for(int i = 0; i < c_maxTextureUnits; i++)
            {
                if(ms_activeID[i] == m_internalID)
                {
                    ms_activeID[i] = 0;
                    ms_activeType[i] = 0U;
                }
            }

            Gl.DeleteTextures(m_internalID);
            m_internalID = 0;
        }

        // OpenGL texture functions
        public void SetCompareFunction(int p_func)
        {
            if(m_internalID == 0U)
                return;

            Bind();
            Gl.TexParameterI(m_internalType, TextureParameterName.TextureCompareFunc, new int[1] { p_func });
        }

        public void SetCompareMode(int p_mode)
        {
            if(m_internalID == 0U)
                return;

            Bind();
            Gl.TexParameterI(m_internalType, TextureParameterName.TextureCompareMode, new int[1] { p_mode });
        }

        public void SetWrap(int p_wrap)
        {
            if(m_internalID == 0U)
                return;

            Bind();
            Gl.TexParameterI(m_internalType, TextureParameterName.TextureWrapS, new int[1] { p_wrap });
            Gl.TexParameterI(m_internalType, TextureParameterName.TextureWrapT, new int[1] { p_wrap });
        }

        public void Bind(TextureUnit p_glSlot = TextureUnit.Texture0)
        {
            if(m_internalID == 0U)
                return;

            int l_slotIndex = p_glSlot - TextureUnit.Texture0;
            if(ms_activeID[l_slotIndex] == m_internalID)
                return;

            Gl.ActiveTexture(p_glSlot);
            Gl.BindTexture(m_internalType, m_internalID);

            ms_activeID[l_slotIndex] = m_internalID;
            ms_activeType[l_slotIndex] = m_internalType;
        }

        internal static void RebindSlots()
        {
            for(int i = 0; i < c_maxTextureUnits; i++)
            {
                if(ms_activeID[i] != 0U)
                {
                    Gl.ActiveTexture(TextureUnit.Texture0 + i);
                    Gl.BindTexture(ms_activeType[i], ms_activeID[i]);
                }
            }
        }
    }
}

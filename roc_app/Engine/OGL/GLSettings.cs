using OpenGL;

namespace ROC.Engine.OGL
{
    internal static class GLSettings
    {
        public static void Set(EnableCap p_setting, bool p_state)
        {
            if(p_state)
                Gl.Enable(p_setting);
            else
                Gl.Disable(p_setting);
        }

        public static void SetDepthMask(bool p_state) => Gl.DepthMask(p_state);

        public static void SetFillMode(PolygonMode p_mode) => Gl.PolygonMode(MaterialFace.FrontAndBack, p_mode);

        public static void SetPixelUnpackAlignment(int p_value) => Gl.PixelStore(PixelStoreParameter.UnpackAlignment, p_value);
    }
}

using OpenGL;
using System;
using System.Collections.Generic;

namespace ROC.Engine.OGL
{
    internal static class GLSettings
    {
        [ThreadStatic]
        static Dictionary<EnableCap, bool> ms_capabilities = new Dictionary<EnableCap, bool>();

        [ThreadStatic]
        static bool ms_depthMask = false;

        public static void Set(EnableCap p_setting, bool p_state)
        {
            if(ms_capabilities.TryGetValue(p_setting, out var l_state))
            {
                if(l_state != p_state)
                {
                    ms_capabilities[p_setting] = p_state;
                    if(p_state)
                        Gl.Enable(p_setting);
                    else
                        Gl.Disable(p_setting);
                }
            }
            else
            {
                ms_capabilities.Add(p_setting, p_state);
                if(p_state)
                    Gl.Enable(p_setting);
                else
                    Gl.Disable(p_setting);
            }
        }

        public static void SetDepthMask(bool p_state)
        {
            if(ms_depthMask != p_state)
            {
                ms_depthMask = p_state;
                Gl.DepthMask(ms_depthMask);
            }
        }

        public static void SetFillMode(PolygonMode p_mode) => Gl.PolygonMode(MaterialFace.FrontAndBack, p_mode);

        public static void SetPixelUnpackAlignment(int p_value) => Gl.PixelStore(PixelStoreParameter.UnpackAlignment, p_value);
    }
}

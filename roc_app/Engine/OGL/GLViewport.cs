using GlmSharp;
using OpenGL;
using System;

namespace ROC.Engine.OGL
{
    internal static class GLViewport
    {
        [ThreadStatic]
        static ivec2 ms_position = ivec2.Zero;

        [ThreadStatic]
        static ivec2 ms_size = ivec2.Zero;

        [ThreadStatic]
        static vec4 ms_clearColor = vec4.UnitW;

        [ThreadStatic]
        static BlendingFactor ms_blendSource = BlendingFactor.SrcAlpha;

        [ThreadStatic]
        static BlendingFactor ms_blendDestination = BlendingFactor.OneMinusSrcAlpha;

        public static ivec2 Position
        {
            get => ms_position;
            set
            {
                ms_position = value;
                Gl.Viewport(ms_position.x, ms_position.y, ms_size.x, ms_size.y);
            }
        }

        public static ivec2 Size
        {
            get => ms_size;
            set
            {
                ms_size = value;
                Gl.Viewport(ms_position.x, ms_position.y, ms_size.x, ms_size.y);
            }
        }

        public static void SetArea(ivec2 p_pos, ivec2 p_size)
        {
            ms_position = p_pos;
            ms_size = p_size;
            Gl.Viewport(ms_position.x, ms_position.y, ms_size.x, ms_size.y);
        }

        public static vec4 ClearColor
        {
            get => ms_clearColor;
            set
            {
                if(ms_clearColor != value)
                {
                    ms_clearColor = value;
                    Gl.ClearColor(ms_clearColor.r, ms_clearColor.g, ms_clearColor.b, ms_clearColor.a);
                }
            }
        }

        public static void SetClearDepth(float p_value) => Gl.ClearDepth(p_value);

        public static void SetBlending(BlendingFactor p_source, BlendingFactor p_destination)
        {
            if(ms_blendSource != p_source || ms_blendDestination != p_destination)
            {
                ms_blendSource = p_source;
                ms_blendDestination = p_destination;
                Gl.BlendFunc(p_source, p_destination);
            }
        }

        public static void Clear(bool p_depth, bool p_color)
        {
            ClearBufferMask l_mode = 0U;

            if(p_depth)
                l_mode |= ClearBufferMask.DepthBufferBit;
            if(p_color)
                l_mode |= ClearBufferMask.ColorBufferBit;

            if(l_mode != 0U)
                Gl.Clear(l_mode);
        }
    }
}

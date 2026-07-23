using GlmSharp;
using System;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;

namespace ROC.Engine
{
    public class EngineException : Exception
    {
        public EngineException()
        {
        }

        public EngineException(string message): base(message)
        {
        }

        public EngineException(string message, Exception inner) : base(message, inner)
        {
        }
    }

    internal static class Utils
    {
        public static string GetEmbeddedResource(string p_name)
        {
            var l_stream = Assembly.GetExecutingAssembly().GetManifestResourceStream("ROC.App.resources." + p_name);
            StreamReader l_streadReader = new StreamReader(l_stream);
            return l_streadReader.ReadToEnd();
        }

        [DllImport("kernel32.dll")]
        public static extern ulong GetTickCount64();

        [DllImport("kernel32.dll")]
        public static extern bool AllocConsole();

        [DllImport("kernel32.dll")]
        public static extern bool FreeConsole();

        [DllImport("kernel32.dll")]
        public static extern IntPtr GetConsoleWindow();

        [DllImport("user32.dll")]
        public static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

        public static bool IsEpsilonEqual(this float a, float b) => (Math.Abs(a - b) < float.Epsilon);

        public static float InverseLerp(uint a, uint b, uint t)
        {
            float l_up = t - a;
            float l_down = b - a;
            return l_up / l_down;
        }

        public static quat SLerpSafe(quat a, quat b, float blend)
        {
            if(blend.IsEpsilonEqual(0f) || Math.Abs(quat.Dot(a, b)).IsEpsilonEqual(1f))
                return a;

            return quat.SLerp(a, b, blend);
        }

        public static float EaseInOut(float p_value) => -0.5f * ((float)Math.Cos(Math.PI * p_value) - 1f);

        public static bool IsBitSet(this byte p_value, byte p_bitMask) => ((p_value & p_bitMask) != 0b0);
    }
}

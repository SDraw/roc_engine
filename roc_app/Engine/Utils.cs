using System;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;

namespace ROC.Engine
{
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
    }
}

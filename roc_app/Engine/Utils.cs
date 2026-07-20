using GlmSharp;
using System;
using System.Collections.Generic;
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

        public static bool IsEpsilonEqual(this float a, float b) => (Math.Abs(a - b) < float.Epsilon);

        public static T[] GetAccessorData<T>(glTFLoader.Schema.Gltf p_model, glTFLoader.Schema.Accessor p_accessor, List<byte[]> p_buffers)
        {
            var l_bufferView = p_model.BufferViews[p_accessor.BufferView.Value];
            int l_elementCount = p_accessor.Count;
            int l_perComponentSize = 1;
            switch(p_accessor.ComponentType)
            {
                case glTFLoader.Schema.Accessor.ComponentTypeEnum.BYTE:
                case glTFLoader.Schema.Accessor.ComponentTypeEnum.UNSIGNED_BYTE:
                    l_perComponentSize = 1;
                    break;

                case glTFLoader.Schema.Accessor.ComponentTypeEnum.SHORT:
                case glTFLoader.Schema.Accessor.ComponentTypeEnum.UNSIGNED_SHORT:
                    l_perComponentSize = 2;
                    break;

                case glTFLoader.Schema.Accessor.ComponentTypeEnum.FLOAT:
                case glTFLoader.Schema.Accessor.ComponentTypeEnum.UNSIGNED_INT:
                    l_perComponentSize = 4;
                    break;
            }
            int l_componentsCount = 1;
            switch(p_accessor.Type)
            {
                case glTFLoader.Schema.Accessor.TypeEnum.SCALAR:
                    l_componentsCount = 1;
                    break;
                case glTFLoader.Schema.Accessor.TypeEnum.VEC2:
                    l_componentsCount = 2;
                    break;
                case glTFLoader.Schema.Accessor.TypeEnum.VEC3:
                    l_componentsCount = 3;
                    break;
                case glTFLoader.Schema.Accessor.TypeEnum.VEC4:
                    l_componentsCount = 4;
                    break;
            }

            T[] l_result = new T[l_elementCount * l_componentsCount];
            Buffer.BlockCopy(p_buffers[l_bufferView.Buffer], l_bufferView.ByteOffset + p_accessor.ByteOffset, l_result, 0, l_elementCount * (l_perComponentSize * l_componentsCount));
            return l_result;
        }

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
    }
}

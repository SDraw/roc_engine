using System;
using System.Collections.Generic;
using System.Numerics;

namespace ROC.Converter
{
    internal static class Utils
    {
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

        public static bool IsEpsilonEqual(this float a, float b) => (Math.Abs(a - b) < float.Epsilon);

        public static float MaxElement(this Vector3 p_vec) => Math.Max(Math.Max(p_vec.X, p_vec.Y), p_vec.Z);
        public static Vector3 Max(Vector3 p_a, Vector3 p_b) => new Vector3(Math.Max(p_a.X, p_b.X), Math.Max(p_a.Y, p_b.Y), Math.Max(p_a.Z, p_b.Z));

        public static byte[] AsBytes(this float[] p_source)
        {
            int l_size = p_source.Length * sizeof(float);
            byte[] l_data = new byte[l_size];
            Buffer.BlockCopy(p_source, 0, l_data, 0, l_size);
            return l_data;

        }

        public static byte[] AsBytes(this int[] p_source)
        {
            int l_size = p_source.Length * sizeof(float);
            byte[] l_data = new byte[l_size];
            Buffer.BlockCopy(p_source, 0, l_data, 0, l_size);
            return l_data;

        }

        public static string GetExtensionByType(glTFLoader.Schema.Image.MimeTypeEnum? p_value)
        {
            if(!p_value.HasValue)
                return "";

            switch(p_value.Value)
            {
                case glTFLoader.Schema.Image.MimeTypeEnum.image_jpeg:
                    return ".jpg";
                case glTFLoader.Schema.Image.MimeTypeEnum.image_png:
                    return ".png";
                default:
                    return "";
            }
        }
    }
}

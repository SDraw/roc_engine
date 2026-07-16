using System;
using System.Runtime.InteropServices;

namespace LuaSharp
{
    public static class Extensions
    {
        public static long GetInt(this IntPtr p_source)
        {
            long l_result = 0;
            try
            {
                l_result = Marshal.ReadInt64(p_source);
            }
            catch(Exception) { }
            return l_result;
        }

        public static void SetInt(this IntPtr p_source, long p_value)
        {
            try
            {
                Marshal.WriteInt64(p_source, p_value);
            }
            catch(Exception) { }
        }

        public static long CombineInts(int left, int right) => ((((long)left) << 32) | (uint)right);
    }
}

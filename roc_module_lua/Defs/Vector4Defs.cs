using GlmSharp;
using LuaSharp.Lua;
using ROC.Module.Wrappers;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class Vector4Defs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_vector4Type = typeof(Vector4);

        static Vector4Defs()
        {
            Definition.m_name = nameof(Vector4);
            Definition.m_constructor = Create;

            Definition.m_staticProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("zero", CreateZero, null),
                ("one", CreateOne, null),
                ("unitX", CreateUnitX, null),
                ("unitY", CreateUnitY, null),
                ("unitZ", CreateUnitZ, null),
                ("unitW", CreateUnitW, null)
            };

            Definition.m_staticMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("Distance", Distance),
                ("Dot", Dot),
                ("Lerp", Lerp),
                ("Reflect", Reflect)
            };

            Definition.m_metaMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("__add", Add),
                ("__sub", Subtract),
                ("__div", Divide),
                ("__mul", Multiply),
                ("__len", GetLength)
            };

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("x", GetX, SetX),
                ("y", GetY, SetY),
                ("z", GetZ, SetZ),
                ("w", GetW, SetW),
                ("r", GetX, SetX),
                ("g", GetY, SetY),
                ("b", GetZ, SetZ),
                ("a", GetW, SetW),
                ("normalized", Normalized, null)
            };
        }

        // Constructor
        static int Create(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.Skip();

            if(!l_argReader.ReadNumber(out float l_x) || !l_argReader.ReadNumber(out float l_y) || !l_argReader.ReadNumber(out float l_z) || !l_argReader.ReadNumber(out float l_w))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector4(l_x, l_y, l_z, l_w), ms_vector4Type);
            return 1;
        }

        // Static properties
        static int CreateZero(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.PushObject(new Vector4(vec4.Zero), ms_vector4Type);
            return 1;
        }

        static int CreateOne(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.PushObject(new Vector4(vec4.Ones), ms_vector4Type);
            return 1;
        }

        static int CreateUnitX(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.PushObject(new Vector4(vec4.UnitX), ms_vector4Type);
            return 1;
        }

        static int CreateUnitY(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.PushObject(new Vector4(vec4.UnitY), ms_vector4Type);
            return 1;
        }

        static int CreateUnitZ(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.PushObject(new Vector4(vec4.UnitZ), ms_vector4Type);
            return 1;
        }

        static int CreateUnitW(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.PushObject(new Vector4(vec4.UnitW), ms_vector4Type);
            return 1;
        }

        // Static methods
        static int Distance(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Vector4 l_vecA) || !l_argReader.ReadObject(out Vector4 l_vecB))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(vec4.Distance(l_vecA.m_vector, l_vecB.m_vector));
            return 1;
        }

        static int Dot(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Vector4 l_vecA) || !l_argReader.ReadObject(out Vector4 l_vecB))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(vec4.Dot(l_vecA.m_vector, l_vecB.m_vector));
            return 1;
        }

        static int Lerp(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Vector4 l_vecA) || !l_argReader.ReadObject(out Vector4 l_vecB) || !l_argReader.ReadNumber(out float l_alpha))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector4(vec4.Lerp(l_vecA.m_vector, l_vecB.m_vector, l_alpha)), ms_vector4Type);
            return 1;
        }

        static int Reflect(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Vector4 l_vecA) || !l_argReader.ReadObject(out Vector4 l_vecB))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector4(vec4.Reflect(l_vecA.m_vector, l_vecB.m_vector)), ms_vector4Type);
            return 1;
        }

        // Metamethods
        static int Add(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadValue(out object l_valA) || !l_argReader.ReadValue(out object l_valB))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            // Vector + value
            if(l_valA is Vector4 l_vec0 && l_valB is double l_val0)
            {
                l_argReader.PushObject(new Vector4(l_vec0.m_vector + (float)l_val0), ms_vector4Type);
                return 1;
            }

            if(l_valA is Vector4 l_vec1 && l_valB is long l_val1)
            {
                l_argReader.PushObject(new Vector4(l_vec1.m_vector + l_val1), ms_vector4Type);
                return 1;
            }

            if(l_valA is Vector4 l_vec2 && l_valB is Vector4 l_val2)
            {
                l_argReader.PushObject(new Vector4(l_vec2.m_vector + l_val2.m_vector), ms_vector4Type);
                return 1;
            }

            // Value + vector
            if(l_valA is double l_val3 && l_valB is Vector4 l_vec3)
            {
                l_argReader.PushObject(new Vector4((float)l_val3 + l_vec3.m_vector), ms_vector4Type);
                return 1;
            }

            if(l_valA is long l_val4 && l_valB is Vector4 l_vec4)
            {
                l_argReader.PushObject(new Vector4(l_val4 + l_vec4.m_vector), ms_vector4Type);
                return 1;
            }

            l_argReader.PushBoolean(false);
            return 1;
        }

        static int Subtract(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadValue(out object l_valA) || !l_argReader.ReadValue(out object l_valB))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            // Vector - value
            if(l_valA is Vector4 l_vec0 && l_valB is double l_val0)
            {
                l_argReader.PushObject(new Vector4(l_vec0.m_vector - (float)l_val0), ms_vector4Type);
                return 1;
            }

            if(l_valA is Vector4 l_vec1 && l_valB is long l_val1)
            {
                l_argReader.PushObject(new Vector4(l_vec1.m_vector - l_val1), ms_vector4Type);
                return 1;
            }

            if(l_valA is Vector4 l_vec2 && l_valB is Vector4 l_val2)
            {
                l_argReader.PushObject(new Vector4(l_vec2.m_vector - l_val2.m_vector), ms_vector4Type);
                return 1;
            }

            // Value - vector
            if(l_valA is double l_val3 && l_valB is Vector4 l_vec3)
            {
                l_argReader.PushObject(new Vector4((float)l_val3 - l_vec3.m_vector), ms_vector4Type);
                return 1;
            }

            if(l_valA is long l_val4 && l_valB is Vector4 l_vec4)
            {
                l_argReader.PushObject(new Vector4(l_val4 - l_vec4.m_vector), ms_vector4Type);
                return 1;
            }

            l_argReader.PushBoolean(false);
            return 1;
        }

        static int Multiply(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadValue(out object l_valA) || !l_argReader.ReadValue(out object l_valB))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            // Vector * value
            if(l_valA is Vector4 l_vec0 && l_valB is double l_val0)
            {
                l_argReader.PushObject(new Vector4(l_vec0.m_vector * (float)l_val0), ms_vector4Type);
                return 1;
            }

            if(l_valA is Vector4 l_vec1 && l_valB is long l_val1)
            {
                l_argReader.PushObject(new Vector4(l_vec1.m_vector * l_val1), ms_vector4Type);
                return 1;
            }

            if(l_valA is Vector4 l_vec2 && l_valB is Vector4 l_val2)
            {
                l_argReader.PushObject(new Vector4(l_vec2.m_vector * l_val2.m_vector), ms_vector4Type);
                return 1;
            }

            // Value * vector
            if(l_valA is double l_val3 && l_valB is Vector4 l_vec3)
            {
                l_argReader.PushObject(new Vector4((float)l_val3 * l_vec3.m_vector), ms_vector4Type);
                return 1;
            }

            if(l_valA is long l_val4 && l_valB is Vector4 l_vec4)
            {
                l_argReader.PushObject(new Vector4(l_val4 * l_vec4.m_vector), ms_vector4Type);
                return 1;
            }

            l_argReader.PushBoolean(false);
            return 1;
        }

        static int Divide(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadValue(out object l_valA) || !l_argReader.ReadValue(out object l_valB))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            // Vector / value
            if(l_valA is Vector4 l_vec0 && l_valB is double l_val0)
            {
                l_argReader.PushObject(new Vector4(l_vec0.m_vector / (float)l_val0), ms_vector4Type);
                return 1;
            }

            if(l_valA is Vector4 l_vec1 && l_valB is long l_val1)
            {
                l_argReader.PushObject(new Vector4(l_vec1.m_vector / l_val1), ms_vector4Type);
                return 1;
            }

            if(l_valA is Vector4 l_vec2 && l_valB is Vector4 l_val2)
            {
                l_argReader.PushObject(new Vector4(l_vec2.m_vector / l_val2.m_vector), ms_vector4Type);
                return 1;
            }

            // value / vector
            if(l_valA is double l_val3 && l_valB is Vector4 l_vec3)
            {
                l_argReader.PushObject(new Vector4((float)l_val3 / l_vec3.m_vector), ms_vector4Type);
                return 1;
            }

            if(l_valA is long l_val4 && l_valB is Vector4 l_vec4)
            {
                l_argReader.PushObject(new Vector4(l_val4 / l_vec4.m_vector), ms_vector4Type);
                return 1;
            }

            l_argReader.PushBoolean(false);
            return 1;
        }

        static int GetLength(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Vector4 l_vec))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_vec.m_vector.Length);
            return 1;
        }

        // Instance properties
        static int GetX(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Vector4 l_vec))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_vec.m_vector.x);
            return 1;
        }

        static int SetX(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Vector4 l_vec) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_vec.m_vector.x = l_val;
            return 0;
        }

        static int GetY(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Vector4 l_vec))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_vec.m_vector.y);
            return 1;
        }

        static int SetY(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Vector4 l_vec) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_vec.m_vector.y = l_val;
            return 0;
        }

        static int GetZ(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Vector4 l_vec))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_vec.m_vector.z);
            return 1;
        }

        static int SetZ(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Vector4 l_vec) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_vec.m_vector.z = l_val;
            return 0;
        }

        static int GetW(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Vector4 l_vec))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_vec.m_vector.w);
            return 1;
        }

        static int SetW(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Vector4 l_vec) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_vec.m_vector.w = l_val;
            return 0;
        }

        static int Normalized(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Vector4 l_vec))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector4(l_vec.m_vector.Normalized), ms_vector4Type);
            return 1;
        }
    }
}

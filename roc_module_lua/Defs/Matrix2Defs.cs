using GlmSharp;
using LuaSharp.Lua;
using ROC.Module.Wrappers;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class Matrix2Defs
    {
        static public readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_Matrix2Type = typeof(Matrix2);
        static readonly Type ms_Vector2Type = typeof(Vector2);

        static Matrix2Defs()
        {
            Definition.m_name = nameof(Matrix2);
            Definition.m_constructor = Create;

            Definition.m_staticProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("zero", CreateZero, null),
                ("identity", CreateIdentity, null)
            };

            Definition.m_metaMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("__add", Add),
                ("__sub", Subtract),
                ("__div", Divide),
                ("__mul", Multiply),
                ("__len", GetDeterminant)
            };

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("inversed", Inverse, null),
                ("adjugated", Adjugate, null),
                ("transposed", Transpose, null),
                ("column0", GetColumn0, SetColumn0),
                ("column1", GetColumn1, SetColumn1),
                ("row0", GetRow0, SetRow0),
                ("row1", GetRow1, SetRow1),
                ("m00", GetM00, SetM00),
                ("m01", GetM01, SetM01),
                ("m10", GetM10, SetM10),
                ("m11", GetM11, SetM11)
            };
        }

        static int Create(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            l_reader.Skip();

            float[] l_values = new float[4];
            for(int i = 0; i < 4; i++)
            {
                if(!l_reader.ReadNumber(out l_values[i]))
                {
                    l_reader.PushBoolean(false);
                    return 1;
                }
            }

            l_reader.PushObject(new Matrix2(new mat2(
                l_values[0], l_values[1],
                l_values[2], l_values[3]
            )), ms_Matrix2Type);
            return 1;
        }

        static int CreateZero(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            l_reader.PushObject(new Matrix2(mat2.Zero), ms_Matrix2Type);
            return 1;
        }

        static int CreateIdentity(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            l_reader.PushObject(new Matrix2(mat2.Identity), ms_Matrix2Type);
            return 1;
        }

        static int Add(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadValue(out object p_objA) || !l_reader.ReadValue(out object l_objB))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            // Matrix2 + value
            if(p_objA is Matrix2 l_mat0 && l_objB is double l_val0)
            {
                l_reader.PushObject(new Matrix2(l_mat0.m_matrix + (float)l_val0), ms_Matrix2Type);
                return 1;
            }
            if(p_objA is Matrix2 l_mat1 && l_objB is long l_val1)
            {
                l_reader.PushObject(new Matrix2(l_mat1.m_matrix + l_val1), ms_Matrix2Type);
                return 1;
            }
            if(p_objA is Matrix2 l_mat2 && l_objB is Matrix2 l_val2)
            {
                l_reader.PushObject(new Matrix2(l_mat2.m_matrix + l_val2.m_matrix), ms_Matrix2Type);
                return 1;
            }

            // Value + matrix
            if(p_objA is double l_val4 && p_objA is Matrix2 l_mat4)
            {
                l_reader.PushObject(new Matrix2((float)l_val4 + l_mat4.m_matrix), ms_Matrix2Type);
                return 1;
            }
            if(p_objA is long l_val5 && p_objA is Matrix2 l_mat5)
            {
                l_reader.PushObject(new Matrix2(l_val5 + l_mat5.m_matrix), ms_Matrix2Type);
                return 1;
            }

            l_reader.PushBoolean(false);
            return 1;
        }

        static int Subtract(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadValue(out object p_objA) || !l_reader.ReadValue(out object l_objB))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            // Matrix2 - value
            if(p_objA is Matrix2 l_mat0 && l_objB is double l_val0)
            {
                l_reader.PushObject(new Matrix2(l_mat0.m_matrix - (float)l_val0), ms_Matrix2Type);
                return 1;
            }
            if(p_objA is Matrix2 l_mat1 && l_objB is long l_val1)
            {
                l_reader.PushObject(new Matrix2(l_mat1.m_matrix - l_val1), ms_Matrix2Type);
                return 1;
            }
            if(p_objA is Matrix2 l_mat2 && l_objB is Matrix2 l_val2)
            {
                l_reader.PushObject(new Matrix2(l_mat2.m_matrix - l_val2.m_matrix), ms_Matrix2Type);
                return 1;
            }

            // Value - matrix
            if(p_objA is double l_val4 && p_objA is Matrix2 l_mat4)
            {
                l_reader.PushObject(new Matrix2((float)l_val4 - l_mat4.m_matrix), ms_Matrix2Type);
                return 1;
            }
            if(p_objA is long l_val5 && p_objA is Matrix2 l_mat5)
            {
                l_reader.PushObject(new Matrix2(l_val5 - l_mat5.m_matrix), ms_Matrix2Type);
                return 1;
            }

            l_reader.PushBoolean(false);
            return 1;
        }


        static int Multiply(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadValue(out object p_objA) || !l_reader.ReadValue(out object l_objB))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            // Matrix2 * value
            if(p_objA is Matrix2 l_mat0 && l_objB is double l_val0)
            {
                l_reader.PushObject(new Matrix2(l_mat0.m_matrix * (float)l_val0), ms_Matrix2Type);
                return 1;
            }
            if(p_objA is Matrix2 l_mat1 && l_objB is long l_val1)
            {
                l_reader.PushObject(new Matrix2(l_mat1.m_matrix * l_val1), ms_Matrix2Type);
                return 1;
            }
            if(p_objA is Matrix2 l_mat2 && l_objB is Matrix2 l_val2)
            {
                l_reader.PushObject(new Matrix2(l_mat2.m_matrix * l_val2.m_matrix), ms_Matrix2Type);
                return 1;
            }
            if(p_objA is Matrix2 l_mat3 && l_objB is Vector2 l_val3)
            {
                l_reader.PushObject(new Vector2(l_mat3.m_matrix * l_val3.m_vector), ms_Vector2Type);
                return 1;
            }

            // Value * matrix
            if(p_objA is double l_val4 && p_objA is Matrix2 l_mat4)
            {
                l_reader.PushObject(new Matrix2((float)l_val4 * l_mat4.m_matrix), ms_Matrix2Type);
                return 1;
            }
            if(p_objA is long l_val5 && p_objA is Matrix2 l_mat5)
            {
                l_reader.PushObject(new Matrix2(l_val5 * l_mat5.m_matrix), ms_Matrix2Type);
                return 1;
            }

            l_reader.PushBoolean(false);
            return 1;
        }

        static int Divide(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadValue(out object p_objA) || !l_reader.ReadValue(out object l_objB))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            // Matrix2 / value
            if(p_objA is Matrix2 l_mat0 && l_objB is double l_val0)
            {
                l_reader.PushObject(new Matrix2(l_mat0.m_matrix / (float)l_val0), ms_Matrix2Type);
                return 1;
            }
            if(p_objA is Matrix2 l_mat1 && l_objB is long l_val1)
            {
                l_reader.PushObject(new Matrix2(l_mat1.m_matrix / l_val1), ms_Matrix2Type);
                return 1;
            }
            if(p_objA is Matrix2 l_mat2 && l_objB is Matrix2 l_val2)
            {
                l_reader.PushObject(new Matrix2(l_mat2.m_matrix / l_val2.m_matrix), ms_Matrix2Type);
                return 1;
            }

            // Value * matrix
            if(p_objA is double l_val4 && p_objA is Matrix2 l_mat4)
            {
                l_reader.PushObject(new Matrix2((float)l_val4 / l_mat4.m_matrix), ms_Matrix2Type);
                return 1;
            }
            if(p_objA is long l_val5 && p_objA is Matrix2 l_mat5)
            {
                l_reader.PushObject(new Matrix2(l_val5 / l_mat5.m_matrix), ms_Matrix2Type);
                return 1;
            }

            l_reader.PushBoolean(false);
            return 1;
        }

        static int GetDeterminant(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushNumber(l_mat.m_matrix.Determinant);
            return 1;
        }

        // Properties
        static int Inverse(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushObject(new Matrix2(l_mat.m_matrix.Inverse), ms_Matrix2Type);
            return 1;
        }

        static int Adjugate(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushObject(new Matrix2(l_mat.m_matrix.Adjugate), ms_Matrix2Type);
            return 1;
        }

        static int Transpose(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushObject(new Matrix2(l_mat.m_matrix.Transposed), ms_Matrix2Type);
            return 1;
        }

        static int GetColumn0(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushObject(new Vector2(l_mat.m_matrix.Column0), ms_Vector2Type);
            return 1;
        }
        static int SetColumn0(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat) || !l_reader.ReadObject(out Vector2 l_vec))
                return 0;

            l_mat.m_matrix.Column0 = l_vec.m_vector;
            return 0;
        }

        static int GetColumn1(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushObject(new Vector2(l_mat.m_matrix.Column1), ms_Vector2Type);
            return 1;
        }
        static int SetColumn1(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat) || !l_reader.ReadObject(out Vector2 l_vec))
                return 0;

            l_mat.m_matrix.Column1 = l_vec.m_vector;
            return 0;
        }

        static int GetRow0(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushObject(new Vector2(l_mat.m_matrix.Row0), ms_Vector2Type);
            return 1;
        }
        static int SetRow0(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat) || !l_reader.ReadObject(out Vector2 l_vec))
                return 0;

            l_mat.m_matrix.Row0 = l_vec.m_vector;
            return 0;
        }

        static int GetRow1(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushObject(new Vector2(l_mat.m_matrix.Row1), ms_Vector2Type);
            return 1;
        }
        static int SetRow1(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat) || !l_reader.ReadObject(out Vector2 l_vec))
                return 0;

            l_mat.m_matrix.Row1 = l_vec.m_vector;
            return 0;
        }

        static int GetM00(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushNumber(l_mat.m_matrix.m00);
            return 1;
        }
        static int SetM00(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat) || !l_reader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m00 = l_val;
            return 0;
        }

        static int GetM01(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushNumber(l_mat.m_matrix.m01);
            return 1;
        }
        static int SetM01(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat) || !l_reader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m01 = l_val;
            return 0;
        }

        static int GetM10(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushNumber(l_mat.m_matrix.m10);
            return 1;
        }
        static int SetM10(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat) || !l_reader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m10 = l_val;
            return 0;
        }

        static int GetM11(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushNumber(l_mat.m_matrix.m11);
            return 1;
        }
        static int SetM11(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Matrix2 l_mat) || !l_reader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m11 = l_val;
            return 0;
        }
    }
}

using GlmSharp;
using LuaSharp.Lua;
using ROC.Module.Wrappers;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class Matrix4Defs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_matrix4Type = typeof(Matrix4);
        static readonly Type ms_vector4Type = typeof(Vector4);

        static Matrix4Defs()
        {
            Definition.m_name = nameof(Matrix4);
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
                ("column2", GetColumn2, SetColumn2),
                ("column3", GetColumn3, SetColumn3),
                ("row0", GetRow0, SetRow0),
                ("row1", GetRow1, SetRow1),
                ("row2", GetRow2, SetRow2),
                ("row3", GetRow3, SetRow3),
                ("m00", GetM00, SetM00),
                ("m01", GetM01, SetM01),
                ("m02", GetM02, SetM02),
                ("m03", GetM03, SetM03),
                ("m10", GetM10, SetM10),
                ("m11", GetM11, SetM11),
                ("m12", GetM12, SetM12),
                ("m13", GetM13, SetM13),
                ("m20", GetM20, SetM20),
                ("m21", GetM21, SetM21),
                ("m22", GetM22, SetM22),
                ("m23", GetM23, SetM23),
                ("m30", GetM30, SetM30),
                ("m31", GetM31, SetM31),
                ("m32", GetM32, SetM32),
                ("m33", GetM33, SetM33)
            };
        }

        static int Create(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.Skip();

            float[] l_values = new float[16];
            for(int i = 0; i < 16; i++)
            {
                if(!l_argReader.ReadNumber(out l_values[i]))
                {
                    l_argReader.PushBoolean(false);
                    return 1;
                }
            }

            l_argReader.PushObject(new Matrix4(new mat4(
                l_values[0], l_values[1], l_values[2], l_values[3],
                l_values[4], l_values[5], l_values[6], l_values[7],
                l_values[8], l_values[9], l_values[10], l_values[11],
                l_values[12], l_values[13], l_values[14], l_values[15]
            )), ms_matrix4Type);
            return 1;
        }

        static int CreateZero(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.PushObject(new Matrix4(mat4.Zero), ms_matrix4Type);
            return 1;
        }

        static int CreateIdentity(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.PushObject(new Matrix4(mat4.Identity), ms_matrix4Type);
            return 1;
        }

        static int Add(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadValue(out object p_objA) || !l_argReader.ReadValue(out object l_objB))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            // Matrix4 + value
            if(p_objA is Matrix4 l_mat0 && l_objB is double l_val0)
            {
                l_argReader.PushObject(new Matrix4(l_mat0.m_matrix + (float)l_val0), ms_matrix4Type);
                return 1;
            }
            if(p_objA is Matrix4 l_mat1 && l_objB is long l_val1)
            {
                l_argReader.PushObject(new Matrix4(l_mat1.m_matrix + l_val1), ms_matrix4Type);
                return 1;
            }
            if(p_objA is Matrix4 l_mat2 && l_objB is Matrix4 l_val2)
            {
                l_argReader.PushObject(new Matrix4(l_mat2.m_matrix + l_val2.m_matrix), ms_matrix4Type);
                return 1;
            }

            // Value + matrix
            if(p_objA is double l_val4 && p_objA is Matrix4 l_mat4)
            {
                l_argReader.PushObject(new Matrix4((float)l_val4 + l_mat4.m_matrix), ms_matrix4Type);
                return 1;
            }
            if(p_objA is long l_val5 && p_objA is Matrix4 l_mat5)
            {
                l_argReader.PushObject(new Matrix4(l_val5 + l_mat5.m_matrix), ms_matrix4Type);
                return 1;
            }

            l_argReader.PushBoolean(false);
            return 1;
        }

        static int Subtract(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadValue(out object p_objA) || !l_argReader.ReadValue(out object l_objB))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            // Matrix4 - value
            if(p_objA is Matrix4 l_mat0 && l_objB is double l_val0)
            {
                l_argReader.PushObject(new Matrix4(l_mat0.m_matrix - (float)l_val0), ms_matrix4Type);
                return 1;
            }
            if(p_objA is Matrix4 l_mat1 && l_objB is long l_val1)
            {
                l_argReader.PushObject(new Matrix4(l_mat1.m_matrix - l_val1), ms_matrix4Type);
                return 1;
            }
            if(p_objA is Matrix4 l_mat2 && l_objB is Matrix4 l_val2)
            {
                l_argReader.PushObject(new Matrix4(l_mat2.m_matrix - l_val2.m_matrix), ms_matrix4Type);
                return 1;
            }

            // Value - matrix
            if(p_objA is double l_val4 && p_objA is Matrix4 l_mat4)
            {
                l_argReader.PushObject(new Matrix4((float)l_val4 - l_mat4.m_matrix), ms_matrix4Type);
                return 1;
            }
            if(p_objA is long l_val5 && p_objA is Matrix4 l_mat5)
            {
                l_argReader.PushObject(new Matrix4(l_val5 - l_mat5.m_matrix), ms_matrix4Type);
                return 1;
            }

            l_argReader.PushBoolean(false);
            return 1;
        }


        static int Multiply(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadValue(out object p_objA) || !l_argReader.ReadValue(out object l_objB))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            // Matrix4 * value
            if(p_objA is Matrix4 l_mat0 && l_objB is double l_val0)
            {
                l_argReader.PushObject(new Matrix4(l_mat0.m_matrix * (float)l_val0), ms_matrix4Type);
                return 1;
            }
            if(p_objA is Matrix4 l_mat1 && l_objB is long l_val1)
            {
                l_argReader.PushObject(new Matrix4(l_mat1.m_matrix * l_val1), ms_matrix4Type);
                return 1;
            }
            if(p_objA is Matrix4 l_mat2 && l_objB is Matrix4 l_val2)
            {
                l_argReader.PushObject(new Matrix4(l_mat2.m_matrix * l_val2.m_matrix), ms_matrix4Type);
                return 1;
            }
            if(p_objA is Matrix4 l_mat3 && l_objB is Vector4 l_val3)
            {
                l_argReader.PushObject(new Vector4(l_mat3.m_matrix * l_val3.m_vector), ms_vector4Type);
                return 1;
            }

            // Value * matrix
            if(p_objA is double l_val4 && p_objA is Matrix4 l_mat4)
            {
                l_argReader.PushObject(new Matrix4((float)l_val4 * l_mat4.m_matrix), ms_matrix4Type);
                return 1;
            }
            if(p_objA is long l_val5 && p_objA is Matrix4 l_mat5)
            {
                l_argReader.PushObject(new Matrix4(l_val5 * l_mat5.m_matrix), ms_matrix4Type);
                return 1;
            }

            l_argReader.PushBoolean(false);
            return 1;
        }

        static int Divide(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadValue(out object p_objA) || !l_argReader.ReadValue(out object l_objB))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            // Matrix4 / value
            if(p_objA is Matrix4 l_mat0 && l_objB is double l_val0)
            {
                l_argReader.PushObject(new Matrix4(l_mat0.m_matrix / (float)l_val0), ms_matrix4Type);
                return 1;
            }
            if(p_objA is Matrix4 l_mat1 && l_objB is long l_val1)
            {
                l_argReader.PushObject(new Matrix4(l_mat1.m_matrix / l_val1), ms_matrix4Type);
                return 1;
            }
            if(p_objA is Matrix4 l_mat2 && l_objB is Matrix4 l_val2)
            {
                l_argReader.PushObject(new Matrix4(l_mat2.m_matrix / l_val2.m_matrix), ms_matrix4Type);
                return 1;
            }

            // Value * matrix
            if(p_objA is double l_val4 && p_objA is Matrix4 l_mat4)
            {
                l_argReader.PushObject(new Matrix4((float)l_val4 / l_mat4.m_matrix), ms_matrix4Type);
                return 1;
            }
            if(p_objA is long l_val5 && p_objA is Matrix4 l_mat5)
            {
                l_argReader.PushObject(new Matrix4(l_val5 / l_mat5.m_matrix), ms_matrix4Type);
                return 1;
            }

            l_argReader.PushBoolean(false);
            return 1;
        }

        static int GetDeterminant(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_mat.m_matrix.Determinant);
            return 1;
        }

        // Properties
        static int Inverse(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Matrix4(l_mat.m_matrix.Inverse), ms_matrix4Type);
            return 1;
        }

        static int Adjugate(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Matrix4(l_mat.m_matrix.Adjugate), ms_matrix4Type);
            return 1;
        }

        static int Transpose(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Matrix4(l_mat.m_matrix.Transposed), ms_matrix4Type);
            return 1;
        }

        static int GetColumn0(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector4(l_mat.m_matrix.Column0), ms_vector4Type);
            return 1;
        }
        static int SetColumn0(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadObject(out Vector4 l_vec))
                return 0;

            l_mat.m_matrix.Column0 = l_vec.m_vector;
            return 0;
        }

        static int GetColumn1(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector4(l_mat.m_matrix.Column1), ms_vector4Type);
            return 1;
        }
        static int SetColumn1(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadObject(out Vector4 l_vec))
                return 0;

            l_mat.m_matrix.Column1 = l_vec.m_vector;
            return 0;
        }

        static int GetColumn2(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector4(l_mat.m_matrix.Column2), ms_vector4Type);
            return 1;
        }
        static int SetColumn2(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadObject(out Vector4 l_vec))
                return 0;

            l_mat.m_matrix.Column2 = l_vec.m_vector;
            return 0;
        }

        static int GetColumn3(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector4(l_mat.m_matrix.Column3), ms_vector4Type);
            return 1;
        }
        static int SetColumn3(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadObject(out Vector4 l_vec))
                return 0;

            l_mat.m_matrix.Column3 = l_vec.m_vector;
            return 0;
        }

        static int GetRow0(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector4(l_mat.m_matrix.Row0), ms_vector4Type);
            return 1;
        }
        static int SetRow0(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadObject(out Vector4 l_vec))
                return 0;

            l_mat.m_matrix.Row0 = l_vec.m_vector;
            return 0;
        }

        static int GetRow1(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector4(l_mat.m_matrix.Row1), ms_vector4Type);
            return 1;
        }
        static int SetRow1(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadObject(out Vector4 l_vec))
                return 0;

            l_mat.m_matrix.Row1 = l_vec.m_vector;
            return 0;
        }

        static int GetRow2(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector4(l_mat.m_matrix.Row2), ms_vector4Type);
            return 1;
        }
        static int SetRow2(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadObject(out Vector4 l_vec))
                return 0;

            l_mat.m_matrix.Row2 = l_vec.m_vector;
            return 0;
        }

        static int GetRow3(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector4(l_mat.m_matrix.Row3), ms_vector4Type);
            return 1;
        }
        static int SetRow3(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadObject(out Vector4 l_vec))
                return 0;

            l_mat.m_matrix.Row3 = l_vec.m_vector;
            return 0;
        }

        static int GetM00(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_mat.m_matrix.m00);
            return 1;
        }
        static int SetM00(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m00 = l_val;
            return 0;
        }

        static int GetM01(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_mat.m_matrix.m01);
            return 1;
        }
        static int SetM01(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m01 = l_val;
            return 0;
        }

        static int GetM02(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_mat.m_matrix.m02);
            return 1;
        }
        static int SetM02(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m02 = l_val;
            return 0;
        }

        static int GetM03(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_mat.m_matrix.m03);
            return 1;
        }
        static int SetM03(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m03 = l_val;
            return 0;
        }

        static int GetM10(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_mat.m_matrix.m10);
            return 1;
        }
        static int SetM10(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m10 = l_val;
            return 0;
        }

        static int GetM11(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_mat.m_matrix.m11);
            return 1;
        }
        static int SetM11(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m11 = l_val;
            return 0;
        }

        static int GetM12(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_mat.m_matrix.m12);
            return 1;
        }
        static int SetM12(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m12 = l_val;
            return 0;
        }

        static int GetM13(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_mat.m_matrix.m13);
            return 1;
        }
        static int SetM13(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m13 = l_val;
            return 0;
        }

        static int GetM20(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_mat.m_matrix.m20);
            return 1;
        }
        static int SetM20(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m20 = l_val;
            return 0;
        }

        static int GetM21(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_mat.m_matrix.m21);
            return 1;
        }
        static int SetM21(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m21 = l_val;
            return 0;
        }

        static int GetM22(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_mat.m_matrix.m22);
            return 1;
        }
        static int SetM22(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m22 = l_val;
            return 0;
        }

        static int GetM23(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_mat.m_matrix.m23);
            return 1;
        }
        static int SetM23(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m23 = l_val;
            return 0;
        }

        static int GetM30(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_mat.m_matrix.m30);
            return 1;
        }
        static int SetM30(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m30 = l_val;
            return 0;
        }

        static int GetM31(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_mat.m_matrix.m31);
            return 1;
        }
        static int SetM31(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m31 = l_val;
            return 0;
        }

        static int GetM32(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_mat.m_matrix.m32);
            return 1;
        }
        static int SetM32(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m32 = l_val;
            return 0;
        }

        static int GetM33(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_mat.m_matrix.m33);
            return 1;
        }
        static int SetM33(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Matrix4 l_mat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_mat.m_matrix.m33 = l_val;
            return 0;
        }
    }
}

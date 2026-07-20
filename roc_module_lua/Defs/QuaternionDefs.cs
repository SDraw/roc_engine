using GlmSharp;
using LuaSharp.Lua;
using ROC.Module.Wrappers;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class QuaternionDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_quaternionType = typeof(Quaternion);
        static readonly Type ms_vector3Type = typeof(Vector3);
        static readonly Type ms_vector4Type = typeof(Vector4);

        static QuaternionDefs()
        {
            Definition.m_name = nameof(Quaternion);
            Definition.m_constructor = Create;

            Definition.m_staticProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("identity", CreateIdentity, null)
            };

            Definition.m_staticMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("FromEuler", FromEuler),
                ("FromAxisAngle", FromAxisAngle),
                ("Cross", Cross),
                ("Dot", Dot),
                ("Lerp", Lerp),
                ("Slerp", SLerp)
            };

            Definition.m_metaMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("__mul", Multiply),
                ("__add", Add),
                ("__sub", Subtract),
                ("__div", Divide)
            };

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("x", GetX,SetX),
                ("y", GetY,SetY),
                ("z", GetZ,SetZ),
                ("w", GetW,SetW),
                ("roll", GetRoll, null),
                ("pitch", GetPitch, null),
                ("yaw", GetYaw, null),
                ("normalized", Normalized, null),
                ("inversed", Inversed, null),
                ("conjugated", Conjugated, null),
            };
        }

        static int Create(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.Skip();

            if(!l_argReader.ReadNumber(out float l_x) || !l_argReader.ReadNumber(out float l_y) || !l_argReader.ReadNumber(out float l_z) || !l_argReader.ReadNumber(out float l_w))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Quaternion(new quat(l_x, l_y, l_z, l_w)), ms_quaternionType);
            return 1;
        }

        static int CreateIdentity(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.PushObject(new Quaternion(quat.Identity), ms_quaternionType);
            return 1;
        }

        static int FromEuler(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(l_argReader.IsNextNumber())
            {
                if(!l_argReader.ReadNumber(out float l_x) || !l_argReader.ReadNumber(out float l_y) || !l_argReader.ReadNumber(out float l_z))
                {
                    l_argReader.PushBoolean(false);
                    return 1;
                }

                l_argReader.PushObject(new Quaternion(new quat(new vec3(l_x, l_y, l_z))), ms_quaternionType);
                return 1;
            }

            if(l_argReader.IsNextObject())
            {
                if(!l_argReader.ReadObject(out Vector3 l_vec))
                {
                    l_argReader.PushBoolean(false);
                    return 1;
                }

                l_argReader.PushObject(new Quaternion(new quat(l_vec.m_vector)), ms_quaternionType);
                return 1;
            }

            l_argReader.PushBoolean(false);
            return 1;
        }

        static int FromAxisAngle(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadNumber(out float l_angle) || !l_argReader.ReadObject(out Vector3 l_axis))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Quaternion(quat.FromAxisAngle(l_angle, l_axis.m_vector)), ms_quaternionType);
            return 1;
        }

        static int Cross(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Quaternion l_quatA) || !l_argReader.ReadObject(out Quaternion l_quatB))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Quaternion(quat.Cross(l_quatA.m_quat, l_quatB.m_quat)), ms_quaternionType);
            return 1;
        }

        static int Dot(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Quaternion l_quatA) || !l_argReader.ReadObject(out Quaternion l_quatB))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(quat.Dot(l_quatA.m_quat, l_quatB.m_quat));
            return 1;
        }

        static int Lerp(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Quaternion l_quatA) || !l_argReader.ReadObject(out Quaternion l_quatB) || !l_argReader.ReadNumber(out float l_alpha))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Quaternion(quat.Lerp(l_quatA.m_quat, l_quatB.m_quat, l_alpha)), ms_quaternionType);
            return 1;
        }

        static int SLerp(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Quaternion l_quatA) || !l_argReader.ReadObject(out Quaternion l_quatB) || !l_argReader.ReadNumber(out float l_alpha))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Quaternion(quat.SLerp(l_quatA.m_quat, l_quatB.m_quat, l_alpha)), ms_quaternionType);
            return 1;
        }

        static int Multiply(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadValue(out object l_objA) || !l_argReader.ReadValue(out object l_objB))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            // Quaternion * value
            if(l_objA is Quaternion l_quat0 && l_objB is double l_val0)
            {
                l_argReader.PushObject(new Quaternion(l_quat0.m_quat * (float)l_val0), ms_quaternionType);
                return 1;
            }
            if(l_objA is Quaternion l_quat1 && l_objB is long l_val1)
            {
                l_argReader.PushObject(new Quaternion(l_quat1.m_quat * l_val1), ms_quaternionType);
                return 1;
            }
            if(l_objA is Quaternion l_quat2 && l_objB is Quaternion l_val2)
            {
                l_argReader.PushObject(new Quaternion(l_quat2.m_quat * l_val2.m_quat), ms_quaternionType);
                return 1;
            }
            if(l_objA is Quaternion l_quat3 && l_objB is Vector3 l_val3)
            {
                l_argReader.PushObject(new Vector3(l_quat3.m_quat * l_val3.m_vector), ms_vector3Type);
                return 1;
            }
            if(l_objA is Quaternion l_quat4 && l_objB is Vector4 l_val4)
            {
                l_argReader.PushObject(new Vector4(l_quat4.m_quat * l_val4.m_vector), ms_vector4Type);
                return 1;
            }

            // Value * quaternion
            if(l_objA is double l_val5 && l_objB is Quaternion l_quat5)
            {
                l_argReader.PushObject(new Quaternion((float)l_val5 * l_quat5.m_quat), ms_quaternionType);
                return 1;
            }
            if(l_objA is long l_val6 && l_objB is Quaternion l_quat6)
            {
                l_argReader.PushObject(new Quaternion(l_val6 * l_quat6.m_quat), ms_quaternionType);
                return 1;
            }
            if(l_objA is Vector3 l_val7 && l_objB is Quaternion l_quat7)
            {
                l_argReader.PushObject(new Vector3(l_val7.m_vector * l_quat7.m_quat), ms_vector3Type);
                return 1;
            }
            if(l_objA is Vector4 l_val8 && l_objB is Quaternion l_quat8)
            {
                l_argReader.PushObject(new Vector4(l_val8.m_vector * l_quat8.m_quat), ms_vector4Type);
                return 1;
            }

            l_argReader.PushBoolean(false);
            return 1;
        }

        static int Add(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadValue(out object l_objA) || !l_argReader.ReadValue(out object l_objB))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            // Quaternion + value
            if(l_objA is Quaternion l_quat0 && l_objB is double l_val0)
            {
                l_argReader.PushObject(new Quaternion(l_quat0.m_quat + (float)l_val0), ms_quaternionType);
                return 1;
            }
            if(l_objA is Quaternion l_quat1 && l_objB is long l_val1)
            {
                l_argReader.PushObject(new Quaternion(l_quat1.m_quat + l_val1), ms_quaternionType);
                return 1;
            }
            if(l_objA is Quaternion l_quat2 && l_objB is Quaternion l_val2)
            {
                l_argReader.PushObject(new Quaternion(l_quat2.m_quat + l_val2.m_quat), ms_quaternionType);
                return 1;
            }

            // Value + quaternion
            if(l_objA is double l_val5 && l_objB is Quaternion l_quat5)
            {
                l_argReader.PushObject(new Quaternion((float)l_val5 + l_quat5.m_quat), ms_quaternionType);
                return 1;
            }
            if(l_objA is long l_val6 && l_objB is Quaternion l_quat6)
            {
                l_argReader.PushObject(new Quaternion(l_val6 + l_quat6.m_quat), ms_quaternionType);
                return 1;
            }

            l_argReader.PushBoolean(false);
            return 1;
        }

        static int Subtract(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadValue(out object l_objA) || !l_argReader.ReadValue(out object l_objB))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            // Quaternion - value
            if(l_objA is Quaternion l_quat0 && l_objB is double l_val0)
            {
                l_argReader.PushObject(new Quaternion(l_quat0.m_quat - (float)l_val0), ms_quaternionType);
                return 1;
            }
            if(l_objA is Quaternion l_quat1 && l_objB is long l_val1)
            {
                l_argReader.PushObject(new Quaternion(l_quat1.m_quat - l_val1), ms_quaternionType);
                return 1;
            }
            if(l_objA is Quaternion l_quat2 && l_objB is Quaternion l_val2)
            {
                l_argReader.PushObject(new Quaternion(l_quat2.m_quat - l_val2.m_quat), ms_quaternionType);
                return 1;
            }

            // Value - quaternion
            if(l_objA is double l_val5 && l_objB is Quaternion l_quat5)
            {
                l_argReader.PushObject(new Quaternion((float)l_val5 - l_quat5.m_quat), ms_quaternionType);
                return 1;
            }
            if(l_objA is long l_val6 && l_objB is Quaternion l_quat6)
            {
                l_argReader.PushObject(new Quaternion(l_val6 - l_quat6.m_quat), ms_quaternionType);
                return 1;
            }

            l_argReader.PushBoolean(false);
            return 1;
        }

        static int Divide(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadValue(out object l_objA) || !l_argReader.ReadValue(out object l_objB))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            // Quaternion / value
            if(l_objA is Quaternion l_quat0 && l_objB is double l_val0)
            {
                l_argReader.PushObject(new Quaternion(l_quat0.m_quat / (float)l_val0), ms_quaternionType);
                return 1;
            }
            if(l_objA is Quaternion l_quat1 && l_objB is long l_val1)
            {
                l_argReader.PushObject(new Quaternion(l_quat1.m_quat / l_val1), ms_quaternionType);
                return 1;
            }

            l_argReader.PushBoolean(false);
            return 1;
        }

        static int GetX(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Quaternion l_quat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_quat.m_quat.x);
            return 1;
        }
        static int SetX(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Quaternion l_quat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_quat.m_quat.x = l_val;
            return 0;
        }

        static int GetY(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Quaternion l_quat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_quat.m_quat.y);
            return 1;
        }
        static int SetY(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Quaternion l_quat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_quat.m_quat.y = l_val;
            return 0;
        }

        static int GetZ(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Quaternion l_quat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_quat.m_quat.z);
            return 1;
        }
        static int SetZ(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Quaternion l_quat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_quat.m_quat.z = l_val;
            return 0;
        }

        static int GetW(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Quaternion l_quat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_quat.m_quat.w);
            return 1;
        }
        static int SetW(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Quaternion l_quat) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_quat.m_quat.w = l_val;
            return 0;
        }

        static int GetPitch(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Quaternion l_quat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_quat.m_quat.Pitch);
            return 1;
        }

        static int GetRoll(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Quaternion l_quat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_quat.m_quat.Roll);
            return 1;
        }

        static int GetYaw(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Quaternion l_quat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_quat.m_quat.Yaw);
            return 1;
        }

        static int Normalized(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Quaternion l_quat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Quaternion(l_quat.m_quat.Normalized), ms_quaternionType);
            return 1;
        }

        static int Inversed(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Quaternion l_quat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Quaternion(l_quat.m_quat.Inverse), ms_quaternionType);
            return 1;
        }

        static int Conjugated(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Quaternion l_quat))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Quaternion(l_quat.m_quat.Conjugate), ms_quaternionType);
            return 1;
        }
    }
}

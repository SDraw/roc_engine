using System;
using System.Collections.Generic;

namespace LuaSharp.Lua
{
    public struct ArgReader
    {
        readonly LuaVM m_luaVM;
        readonly int m_top;
        int m_index;
        int m_return;
        bool m_error;

        public ArgReader(IntPtr p_state)
        {
            m_luaVM = LuaVM.GetVM(p_state);
            m_top = m_luaVM.GetTop();
            m_index = Math.Min(1, m_top);
            m_return = 0;
            m_error = false;
        }

        public bool HasError => m_error;

        public int ReturnCount => m_return;

        public void Skip(int p_count = 1)
        {
            if(m_error)
                return;

            m_index += p_count;
        }

        // Read
        public bool ReadBoolean(out bool p_value)
        {
            p_value = false;

            if(m_error)
                return false;

            if(m_index > m_top)
            {
                m_luaVM.SendWarning(string.Format("Expected {0} argument(s), got {1}", m_top, m_index));
                m_error = true;
                return false;
            }

            if(!m_luaVM.IsBoolean(m_index))
            {
                m_luaVM.SendWarning(string.Format("Expected boolean at argument {0}", m_index));
                m_error = true;
                return false;
            }

            p_value = m_luaVM.GetBoolean(m_index);
            m_index++;
            return true;
        }

        public bool ReadNumber(out float p_value)
        {
            p_value = 0f;

            if(m_error)
                return false;

            if(m_index > m_top)
            {
                m_luaVM.SendWarning(string.Format("Expected {0} argument(s), got {1}", m_top, m_index));
                m_error = true;
                return false;
            }

            if(!m_luaVM.IsNumber(m_index))
            {
                m_luaVM.SendWarning(string.Format("Expected number at argument {0}", m_index));
                m_error = true;
                return false;
            }

            p_value = (float)m_luaVM.GetNumber(m_index);
            m_index++;
            return true;
        }

        public bool ReadNumber(out double p_value)
        {
            p_value = 0.0;

            if(m_error)
                return false;

            if(m_index > m_top)
            {
                m_luaVM.SendWarning(string.Format("Expected {0} argument(s), got {1}", m_top, m_index));
                m_error = true;
                return false;
            }

            if(!m_luaVM.IsNumber(m_index))
            {
                m_luaVM.SendWarning(string.Format("Expected number at argument {0}", m_index));
                m_error = true;
                return false;
            }

            p_value = m_luaVM.GetNumber(m_index);
            m_index++;
            return true;
        }

        public bool ReadInteger(out int p_value)
        {
            p_value = 0;

            if(m_error)
                return false;

            if(m_index > m_top)
            {
                m_luaVM.SendWarning(string.Format("Expected {0} argument(s), got {1}", m_top, m_index));
                m_error = true;
                return false;
            }

            if(!m_luaVM.IsNumber(m_index))
            {
                m_luaVM.SendWarning(string.Format("Expected integer at argument {0}", m_index));
                m_error = true;
                return false;
            }

            p_value = (int)m_luaVM.GetInteger(m_index);
            m_index++;
            return true;
        }

        public bool ReadInteger(out long p_value)
        {
            p_value = 0;

            if(m_error)
                return false;

            if(m_index > m_top)
            {
                m_luaVM.SendWarning(string.Format("Expected {0} argument(s), got {1}", m_top, m_index));
                m_error = true;
                return false;
            }

            if(!m_luaVM.IsNumber(m_index))
            {
                m_luaVM.SendWarning(string.Format("Expected integer at argument {0}", m_index));
                m_error = true;
                return false;
            }

            p_value = m_luaVM.GetInteger(m_index);
            m_index++;
            return true;
        }

        public bool ReadString(out string p_value)
        {
            p_value = null;

            if(m_error)
                return false;

            if(m_index > m_top)
            {
                m_luaVM.SendWarning(string.Format("Expected {0} argument(s), got {1}", m_top, m_index));
                m_error = true;
                return false;
            }

            if(!m_luaVM.IsString(m_index))
            {
                m_luaVM.SendWarning(string.Format("Expected string at argument {0}", m_index));
                m_error = true;
                return false;
            }

            p_value = m_luaVM.GetString(m_index);
            m_index++;
            return true;
        }

        public bool ReadObject<T>(out T p_value) where T : class
        {
            p_value = null;

            if(m_error)
                return false;

            if(m_index > m_top)
            {
                m_luaVM.SendWarning(string.Format("Expected {0} argument(s), got {1}", m_top, m_index));
                m_error = true;
                return false;
            }

            if(!m_luaVM.IsObject(m_index))
            {
                m_luaVM.SendWarning(string.Format("Expected object at argument {0}", m_index));
                m_error = true;
                return false;
            }

            if(!m_luaVM.GetObject(m_index, out p_value))
            {
                m_luaVM.SendWarning(string.Format("Expected {0} at argument {1}", typeof(T).Name, m_index));
                m_error = true;
                return false;
            }

            m_index++;
            return true;
        }

        public bool ReadEnum<T>(out T p_result) where T : struct
        {
            p_result = default;

            if(m_error)
                return false;

            if(m_index > m_top)
            {
                m_luaVM.SendWarning(string.Format("Expected {0} argument(s), got {1}", m_top, m_index));
                m_error = true;
                return false;
            }

            if(!m_luaVM.IsString(m_index))
            {
                m_luaVM.SendWarning(string.Format("Expected string at argument {0}", m_index));
                m_error = true;
                return false;
            }

            if(!Enum.TryParse(m_luaVM.GetString(m_index), out p_result))
            {
                m_luaVM.SendWarning(string.Format("Invalid enum value as string at argument {0}", m_index));
                m_error = true;
                return false;
            }

            m_index++;
            return true;
        }

        public bool ReadValue(out object p_value)
        {
            p_value = null;

            if(m_error)
                return false;

            if(m_index > m_top)
            {
                m_luaVM.SendWarning(string.Format("Expected {0} argument(s), got {1}", m_top, m_index));
                m_error = true;
                return false;
            }

            p_value = m_luaVM.GetValue(m_index);
            m_index++;
            return true;
        }

        // Push
        public void PushBoolean(bool p_value)
        {
            m_luaVM.PushBoolean(p_value);
            m_return++;
        }

        public void PushNumber(float p_value)
        {
            m_luaVM.PushNumber(p_value);
            m_return++;
        }

        public void PushNumber(double p_value)
        {
            m_luaVM.PushNumber(p_value);
            m_return++;
        }

        public void PushInteger(int p_value)
        {
            m_luaVM.PushInteger(p_value);
            m_return++;
        }

        public void PushInteger(long p_value)
        {
            m_luaVM.PushInteger(p_value);
            m_return++;
        }

        public void PushString(string p_str)
        {
            m_luaVM.PushString(p_str);
            m_return++;
        }

        public void PushObject<T>(T p_obj) where T : class
        {
            m_luaVM.PushObject(p_obj);
            m_return++;
        }

        public void PushObject<T>(T p_obj, Type p_type) where T : class
        {
            m_luaVM.PushObject(p_obj, p_type);
            m_return++;
        }

        public void PushNil()
        {
            m_luaVM.PushNil();
            m_return++;
        }

        public void PushValue(object p_value)
        {
            m_luaVM.PushValue(p_value);
            m_return++;
        }

        // Tables
        public void PushTable<T>(T[] p_table)
        {
            m_luaVM.PushTable(p_table);
            m_return++;
        }

        public void PushTable<T>(List<T> p_table)
        {
            m_luaVM.PushTable(p_table);
            m_return++;
        }

        public void PushTable(Dictionary<string, object> p_table)
        {
            m_luaVM.PushTable(p_table);
            m_return++;
        }

        // Check
        public bool IsNextBoolean() => ((m_index <= m_top) && m_luaVM.IsBoolean(m_index));
        public bool IsNextNumber() => ((m_index <= m_top) && m_luaVM.IsNumber(m_index));
        public bool IsNextInteger() => ((m_index <= m_top) && m_luaVM.IsInteger(m_index));
        public bool IsNextString() => ((m_index <= m_top) && m_luaVM.IsString(m_index));
        public bool IsNextObject() => ((m_index <= m_top) && m_luaVM.IsObject(m_index));
        public bool IsNextNil() => ((m_index <= m_top) && m_luaVM.IsNil(m_index));

        // Custom warning
        public void SetWarning(string p_warning) => m_luaVM.SendWarning(p_warning);
    }
}

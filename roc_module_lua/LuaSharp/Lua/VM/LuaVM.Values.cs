using System;

namespace LuaSharp.Lua
{
    public sealed partial class LuaVM : IDisposable
    {
        // Extended reads/pushes
        public object GetValue(int p_index)
        {
            object l_result = null;
            if(LuaInterop.lua_isinteger(m_state, p_index) == 1)
                l_result = LuaInterop.lua_tointeger(m_state, p_index);
            else
            {
                switch(LuaInterop.lua_type(m_state, p_index))
                {
                    case LuaInterop.LUA_TBOOLEAN:
                        l_result = (LuaInterop.lua_toboolean(m_state, p_index) == 1);
                        break;

                    case LuaInterop.LUA_TNUMBER:
                        l_result = LuaInterop.lua_tonumber(m_state, p_index);
                        break;

                    case LuaInterop.LUA_TSTRING:
                        l_result = LuaInterop.lua_tostring(m_state, p_index);
                        break;

                    case LuaInterop.LUA_TUSERDATA:
                    {
                        long l_hash = LuaInterop.lua_touserdata(m_state, p_index).GetInt();
                        if(m_objectsMap.TryGetValue(l_hash, out var l_refObj))
                            l_result = l_refObj;
                    }
                    break;
                }
            }
            return l_result;
        }


        public void PushValue(object p_value) => PushValue(p_value, p_value?.GetType());
        public void PushValue(object p_value, Type p_type)
        {
            // Always pushes something
            if(p_value == null)
            {
                PushNil();
                return;
            }

            switch(Type.GetTypeCode(p_type))
            {
                case TypeCode.Boolean:
                    PushBoolean((bool)p_value);
                    break;

                case TypeCode.UInt32:
                    PushInteger((uint)p_value);
                    break;

                case TypeCode.Int32:
                    PushInteger((int)p_value);
                    break;

                case TypeCode.UInt64:
                    PushInteger((long)(ulong)p_value);
                    break;

                case TypeCode.Int64:
                    PushInteger((long)p_value);
                    break;

                case TypeCode.Single:
                    PushNumber((float)p_value);
                    break;

                case TypeCode.Double:
                    PushNumber((double)p_value);
                    break;

                case TypeCode.String:
                    PushString((string)p_value);
                    break;

                case TypeCode.Object:
                {
                    if(p_type.IsClass)
                        PushObject(p_value);
                    else
                        PushNil();
                }
                break;

                default:
                    PushNil();
                    break;
            }
        }

        public void SetValue(string p_name, object p_obj)
        {
            PushValue(p_obj);
            LuaInterop.lua_setglobal(m_state, p_name);
        }
    }
}

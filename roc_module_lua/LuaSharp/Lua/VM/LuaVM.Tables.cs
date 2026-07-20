using System;
using System.Collections.Generic;

namespace LuaSharp.Lua
{
    public sealed partial class LuaVM : IDisposable
    {
        public void PushTable<T>(T[] p_array)
        {
            LuaInterop.lua_newtable(m_state);
            for(int i = 0, j = p_array.Length; i < j; i++)
            {
                LuaInterop.lua_pushinteger(m_state, i + 1);
                PushValue(p_array[i]);
                LuaInterop.lua_settable(m_state, -3);
            }
        }

        public void PushTable<T>(List<T> p_list)
        {
            LuaInterop.lua_newtable(m_state);
            long l_index = 1;
            foreach(object l_obj in p_list)
            {
                LuaInterop.lua_pushinteger(m_state, l_index);
                PushValue(l_obj);
                LuaInterop.lua_settable(m_state, -3);
                l_index++;
            }
        }

        public void PushTable<T>(Dictionary<string, T> p_list)
        {
            LuaInterop.lua_newtable(m_state);
            foreach(var l_pair in p_list)
            {
                PushValue(l_pair.Value);
                LuaInterop.lua_setfield(m_state, -2, l_pair.Key);
            }
        }
    }
}

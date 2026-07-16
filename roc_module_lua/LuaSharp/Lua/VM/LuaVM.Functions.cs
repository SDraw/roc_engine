using System;
using System.Collections.Generic;

namespace LuaSharp.Lua
{
    public sealed partial class LuaVM : IDisposable
    {
        public bool IsFunctionPresent(string p_name)
        {
            bool l_result = (LuaInterop.lua_getglobal(m_state, p_name) == LuaInterop.LUA_TFUNCTION);
            LuaInterop.lua_pop(m_state, 1);
            return l_result;
        }

        public void SetFunction(string p_name, LuaInterop.lua_CFunction p_func) => LuaInterop.lua_register(m_state, p_name, p_func);

        public int GetFunctionRef(string p_name)
        {
            int l_result = 0;

            if(LuaInterop.lua_getglobal(m_state, p_name) == LuaInterop.LUA_TFUNCTION)
                l_result = LuaInterop.luaL_ref(m_state, LuaInterop.LUA_REGISTRYINDEX);
            else
                LuaInterop.lua_pop(m_state, 1);

            return l_result;
        }

        public int GetFunctionRef(int p_index)
        {
            int l_result = 0;
            if(LuaInterop.lua_isfunction(m_state, p_index))
            {
                LuaInterop.lua_pushvalue(m_state, p_index);
                l_result = LuaInterop.luaL_ref(m_state, LuaInterop.LUA_REGISTRYINDEX);
            }
            return l_result;
        }

        public void ClearFunctionRef(int p_ref) => LuaInterop.luaL_unref(m_state, LuaInterop.LUA_REGISTRYINDEX, p_ref);

        public void CallFunction(int p_ref, params object[] p_args)
        {
            if(LuaInterop.lua_rawgeti(m_state, LuaInterop.LUA_REGISTRYINDEX, p_ref) == LuaInterop.LUA_TFUNCTION)
            {
                foreach(var l_value in p_args)
                    PushValue(l_value);
                if(LuaInterop.lua_pcall(m_state, p_args.Length, 0, 0) != 0)
                {
                    ErrorMessage.Invoke(LuaInterop.lua_tostring(m_state, -1));
                    LuaInterop.lua_pop(m_state, 1);
                }
            }
            else
                LuaInterop.lua_pop(m_state, 1);
        }

        public void CallFunction(int p_ref, List<object> p_results, params object[] p_args)
        {
            if(LuaInterop.lua_rawgeti(m_state, LuaInterop.LUA_REGISTRYINDEX, p_ref) == LuaInterop.LUA_TFUNCTION)
            {
                int l_top = LuaInterop.lua_gettop(m_state);
                foreach(var l_value in p_args)
                    PushValue(l_value);
                if(LuaInterop.lua_pcall(m_state, p_args.Length, LuaInterop.LUA_MULTRET, 0) != LuaInterop.LUA_OK)
                {
                    ErrorMessage.Invoke(LuaInterop.lua_tostring(m_state, -1));
                    LuaInterop.lua_pop(m_state, 1);
                }
                else
                {
                    for(int i = l_top, j = LuaInterop.lua_gettop(m_state); i <= j; i++)
                        p_results.Add(GetValue(i));
                    if(p_results.Count > 0)
                        LuaInterop.lua_pop(m_state, p_results.Count);
                }
            }
            else
                LuaInterop.lua_pop(m_state, 1);
        }
    }
}

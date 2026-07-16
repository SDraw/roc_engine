using System;

namespace LuaSharp.Lua
{
    public sealed partial class LuaVM : IDisposable
    {
        public int GetTop() => LuaInterop.lua_gettop(m_state);

        public bool IsBoolean(int p_index) => LuaInterop.lua_isboolean(m_state, p_index);
        public bool GetBoolean(int p_index) => (LuaInterop.lua_toboolean(m_state, p_index) == 1);
        public void PushBoolean(bool p_val) => LuaInterop.lua_pushboolean(m_state, p_val ? 1 : 0);

        public bool IsInteger(int p_index) => (LuaInterop.lua_isinteger(m_state, p_index) == 1);
        public long GetInteger(int p_index) => LuaInterop.lua_tointeger(m_state, p_index);
        public void PushInteger(long p_val) => LuaInterop.lua_pushinteger(m_state, p_val);

        public bool IsNumber(int p_index) => LuaInterop.lua_isnumber(m_state, p_index);
        public double GetNumber(int p_index) => LuaInterop.lua_tonumber(m_state, p_index);
        public void PushNumber(double p_val) => LuaInterop.lua_pushnumber(m_state, p_val);

        public bool IsString(int p_index) => LuaInterop.lua_isstring(m_state, p_index);
        public string GetString(int p_index) => LuaInterop.lua_tostring(m_state, p_index);
        public void PushString(string p_str) => LuaInterop.lua_pushstring(m_state, p_str);

        public bool IsUserdata(int p_index) => LuaInterop.lua_isuserdata(m_state, p_index);
        public IntPtr GetUserdata(int p_index) => LuaInterop.lua_touserdata(m_state, p_index);

        public bool IsNil(int p_index) => LuaInterop.lua_isnil(m_state, p_index);
        public void PushNil() => LuaInterop.lua_pushnil(m_state);

        public bool IsFunction(int p_index) => LuaInterop.lua_isfunction(m_state, p_index);
        public void PushFunction(LuaInterop.lua_CFunction p_func) => LuaInterop.lua_pushcfunction(m_state, p_func);

        public int GetTypeFromIndex(int p_index) => LuaInterop.lua_type(m_state, p_index);
    }
}

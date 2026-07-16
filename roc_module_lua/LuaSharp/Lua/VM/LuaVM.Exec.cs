using System;

namespace LuaSharp.Lua
{
    public sealed partial class LuaVM : IDisposable
    {
        // Execution of plain script
        public void Execute(string p_script)
        {
            if((LuaInterop.luaL_loadstring(m_state, p_script) != LuaInterop.LUA_OK) || (LuaInterop.lua_pcall(m_state, 0, 0, 0) != LuaInterop.LUA_OK))
            {
                ErrorMessage.Invoke(LuaInterop.lua_tostring(m_state, -1));
                LuaInterop.lua_pop(m_state, 1);
            }
        }

        // Execution of compiled script
        public void Execute(string p_blockName, byte[] p_data)
        {
            if((LuaInterop.luaL_loadbuffer(m_state, ref p_data, p_data.Length, p_blockName) != LuaInterop.LUA_OK) || (LuaInterop.lua_pcall(m_state, 0, 0, 0) != LuaInterop.LUA_OK))
            {
                ErrorMessage.Invoke(LuaInterop.lua_tostring(m_state, -1));
                LuaInterop.lua_pop(m_state, 1);
            }
        }
    }
}

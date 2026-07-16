using System;

namespace LuaSharp.Lua
{
    public sealed partial class LuaVM : IDisposable
    {
        // Garbage collection
        public void PerformStepGC()
        {
            LuaInterop.lua_gc(m_state, LuaInterop.LUA_GCSTEP, 0);
        }

        public void PerformFullGC()
        {
            LuaInterop.lua_gc(m_state, LuaInterop.LUA_GCCOLLECT, 0);
        }
    }
}

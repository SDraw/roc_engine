using System;
using System.Collections.Generic;

namespace LuaSharp.Lua
{
    public sealed partial class LuaVM : IDisposable
    {
        IntPtr m_state = IntPtr.Zero;
        static readonly Dictionary<IntPtr, LuaVM> ms_stateToVM = new Dictionary<IntPtr, LuaVM>();

        // CSharp's GC is stinky
        static readonly LuaInterop.lua_CFunction ms_nilFunctionDelegate = NilResultFunction;

        public LuaVM()
        {
            m_state = LuaInterop.luaL_newstate();

            LuaInterop.luaL_requiref(m_state, "_G", LuaInterop.luaopen_base, 1);
            LuaInterop.luaL_requiref(m_state, "coroutine", LuaInterop.luaopen_coroutine, 1);
            LuaInterop.luaL_requiref(m_state, "table", LuaInterop.luaopen_table, 1);
            LuaInterop.luaL_requiref(m_state, "string", LuaInterop.luaopen_string, 1);
            LuaInterop.luaL_requiref(m_state, "math", LuaInterop.luaopen_math, 1);
            LuaInterop.luaL_requiref(m_state, "utf8", LuaInterop.luaopen_utf8, 1);

            m_objectsMap = new Dictionary<long, object>();

            // Table weak values
            LuaInterop.lua_newtable(m_state);
            LuaInterop.lua_newtable(m_state);
            LuaInterop.lua_pushstring(m_state, "v");
            LuaInterop.lua_setfield(m_state, -2, "__mode");
            LuaInterop.lua_pushcfunction(m_state, ms_nilFunctionDelegate);
            LuaInterop.lua_setfield(m_state, -2, "__metatable");
            LuaInterop.lua_setmetatable(m_state, -2); // Combines two previous tables
            LuaInterop.lua_setfield(m_state, LuaInterop.LUA_REGISTRYINDEX, c_objectsPool);

            ms_stateToVM.Add(m_state, this);
        }

        public void Dispose()
        {
            if(m_state != IntPtr.Zero)
            {
                PerformFullGC();
                LuaInterop.lua_close(m_state);
                m_state = IntPtr.Zero;
            }
        }

        // VM separation
        public static LuaVM GetVM(IntPtr p_state)
        {
            ms_stateToVM.TryGetValue(p_state, out LuaVM l_result);
            return l_result;
        }

        static int NilResultFunction(IntPtr p_state)
        {
            LuaInterop.lua_pushnil(p_state);
            return 1;
        }
    }
}

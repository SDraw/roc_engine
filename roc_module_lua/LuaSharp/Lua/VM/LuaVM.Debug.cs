using System;

namespace LuaSharp.Lua
{
    public sealed partial class LuaVM : IDisposable
    {
        public sealed class MessageEvent<T1>
        {
            event Action<T1> m_action;
            public void AddListener(Action<T1> p_listener) => m_action += p_listener;
            public void RemoveListener(Action<T1> p_listener) => m_action -= p_listener;
            internal void Invoke(T1 p_obj) => m_action?.Invoke(p_obj);
        }

        public readonly MessageEvent<string> ErrorMessage = new MessageEvent<string>();
        public readonly MessageEvent<string> WarningMessage = new MessageEvent<string>();

        // Debug
        public void GetStackInfo(out string p_chunk, out int p_line)
        {
            for(int i = 0; ; i++)
            {
                LuaInterop.lua_Debug l_debug = new LuaInterop.lua_Debug();
                if(LuaInterop.lua_getstack(m_state, i, ref l_debug) != 1)
                {
                    p_chunk = "unknown";
                    p_line = 0;
                    break;
                }

                LuaInterop.lua_getinfo(m_state, "Sl", ref l_debug);
                if(l_debug.currentline < 0)
                    continue;

                p_chunk = System.Runtime.InteropServices.Marshal.PtrToStringAnsi(l_debug.source);
                p_line = l_debug.currentline;
                break;
            }
        }

        // Warning
        public void SendWarning(string p_warning)
        {
            GetStackInfo(out string p_chunk, out int p_line);
            WarningMessage.Invoke(string.Format("[{0}:{1}] Warning: {2}", p_chunk, p_line, p_warning));
        }
    }
}

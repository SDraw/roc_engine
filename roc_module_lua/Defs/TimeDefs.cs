using LuaSharp.Lua;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class TimeDefs
    {
        static public readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();

        static TimeDefs()
        {
            Definition.m_name = "Time";

            Definition.m_staticProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("time", GetTime, null),
                ("deltaTime", GetDeltaTime, null)
            };
        }

        static int GetTime(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            l_reader.PushNumber(Engine.Core.Core.Instance.WindowManager.Time);
            return 1;
        }

        static int GetDeltaTime(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            l_reader.PushNumber(Engine.Core.DeltaTick.Delta);
            return 1;
        }
    }
}

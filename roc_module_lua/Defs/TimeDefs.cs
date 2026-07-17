using LuaSharp.Lua;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class TimeDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();

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
            var l_argReader = new ArgReader(p_state);
            l_argReader.PushNumber(Engine.Core.Core.Instance.WindowManager.Time);
            return 1;
        }

        static int GetDeltaTime(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.PushNumber(Engine.Core.DeltaTick.Delta);
            return 1;
        }
    }
}

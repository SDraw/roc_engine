using LuaSharp.Lua;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class UtilitiesDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();

        static UtilitiesDefs()
        {
            Definition.m_name = "Utilities";

            Definition.m_staticMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("Log", Log)
            };
        }

        static int Log(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadString(out string l_msg))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            Engine.Core.Core.Instance.LogManager.Log(l_msg);
            l_argReader.PushBoolean(true);
            return 1;
        }
    }
}

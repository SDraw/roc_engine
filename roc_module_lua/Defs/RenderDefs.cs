using LuaSharp.Lua;
using ROC.Engine.Managers;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class RenderDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();

        static RenderDefs()
        {
            Definition.m_name = "Render";

            Definition.m_staticProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("renderMode", GetRenderMode, SetRenderMode)
            };
        }

        static int GetRenderMode(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.PushString(Engine.Core.Core.Instance.RenderManager.Mode.ToString());
            return 1;
        }
        static int SetRenderMode(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadEnum(out RenderManager.RenderMode l_val))
                return 0;

            Engine.Core.Core.Instance.RenderManager.Mode = l_val;
            return 0;
        }
    }
}

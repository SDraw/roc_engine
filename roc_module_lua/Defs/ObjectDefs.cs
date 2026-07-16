using LuaSharp.Lua;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class ObjectDefs
    {
        static public readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();

        static ObjectDefs()
        {
            Definition.m_name = nameof(Engine.Objects.Object);
            Definition.m_constructor = null;

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", GetInstanceID, null)
            };
        }

        internal static int GetInstanceID(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Engine.Objects.Object l_go))
            {
                l_reader.PushBoolean(false);
                return l_reader.ReturnCount;
            }

            l_reader.PushInteger((long)l_go.InstanceID);
            return l_reader.ReturnCount;
        }
    }
}

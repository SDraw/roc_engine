using LuaSharp.Lua;
using ROC.Engine.Objects.Resources;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class ResourceDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();

        static ResourceDefs()
        {
            Definition.m_name = nameof(Resource);

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isValid", ObjectDefs.IsValid, null),

                ("log", Log, null)
            };
        }

        internal static int Log(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Resource l_resource))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushString(l_resource.Log);
            return 1;
        }
    }
}

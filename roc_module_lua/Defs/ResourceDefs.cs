using LuaSharp.Lua;
using ROC.Engine.Objects.Resources;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class ResourceDefs
    {
        static public readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();

        static ResourceDefs()
        {
            Definition.m_name = nameof(Resource);

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),

                ("isLoaded", IsLoaded, null),
                ("log", Log, null)
            };

            Definition.m_instanceMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("Unload", Unload)
            };
        }

        internal static int IsLoaded(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Resource l_resource))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushBoolean(l_resource.IsLoaded);
            return 1;
        }

        internal static int Log(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Resource l_resource))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushString(l_resource.Log);
            return 1;
        }

        internal static int Unload(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Resource l_resource))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_resource.Unload();
            l_reader.PushBoolean(true);
            return 1;
        }
    }
}

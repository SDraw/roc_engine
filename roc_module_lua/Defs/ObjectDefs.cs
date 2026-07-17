using LuaSharp.Lua;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class ObjectDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();

        static ObjectDefs()
        {
            Definition.m_name = nameof(Engine.Objects.Object);
            Definition.m_constructor = null;

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", GetInstanceID, null),
                ("isValid", IsValid, null)
            };

            Definition.m_staticMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("IsValid", IsValidStatic),
                ("Destroy", Destroy),
            };
        }

        internal static int GetInstanceID(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Engine.Objects.Object l_obj))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushInteger((long)l_obj.InstanceID);
            return 1;
        }

        internal static int IsValid(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Engine.Objects.Object l_obj))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushBoolean(l_obj.IsValid);
            return 1;
        }

        internal static int IsValidStatic(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.IsNextObject())
            {
                l_argReader.PushBoolean(false);
                return 1;
            }
            if(!l_argReader.ReadObject(out Engine.Objects.Object l_obj))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushBoolean(l_obj.IsValid);
            return 1;
        }

        internal static int Destroy(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Engine.Objects.Object l_obj))
            {
                l_argReader.PushBoolean(false);
                return l_argReader.ReturnCount;
            }

            Engine.Objects.Object.Destroy(l_obj);
            l_argReader.PushBoolean(true);
            return 1;
        }
    }
}

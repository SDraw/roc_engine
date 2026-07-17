using LuaSharp.Lua;
using ROC.Engine.Objects.Components;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class AudioListenerDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();

        static AudioListenerDefs()
        {
            Definition.m_name = nameof(AudioListener);

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isValid", ObjectDefs.IsValid, null),

                ("type", ComponentDefs.GetComponentType, null),
                ("priority", ComponentDefs.GetPriority, null),
                ("enabled", ComponentDefs.GetEnabled, ComponentDefs.SetEnabled),
                ("gameObject", ComponentDefs.GetGameObject, null)
            };
        }
    }
}

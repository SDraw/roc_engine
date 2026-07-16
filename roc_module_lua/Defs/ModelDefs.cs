using LuaSharp.Lua;
using ROC.Engine.Objects.Resources;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class ModelDefs
    {
        static public readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_modelType = typeof(Model);

        static ModelDefs()
        {
            Definition.m_name = nameof(Model);
            Definition.m_constructor = Create;

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isLoaded", ResourceDefs.IsLoaded, null),
                ("log", ResourceDefs.Log, null),

                ("boundsRadius", GetBoundsRadius, null),
                ("type", GetTypeOfMode, null)
            };

            Definition.m_instanceMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("Unload", ResourceDefs.Unload)
            };
        }

        static int Create(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            l_reader.Skip();

            if(!l_reader.ReadString(out string l_path))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushObject(Model.Import(l_path), ms_modelType);
            return 1;
        }

        static int GetBoundsRadius(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Model l_model))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushNumber(l_model.BoundsRadius);
            return 1;
        }

        static int GetTypeOfMode(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Model l_model))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushString(l_model.TypeOfModel.ToString());
            return 1;
        }
    }
}

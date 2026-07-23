using LuaSharp.Lua;
using ROC.Engine.Objects.Resources;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class ModelDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_modelType = typeof(Model);

        static ModelDefs()
        {
            Definition.m_name = nameof(Model);
            Definition.m_constructor = Create;

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isValid", ObjectDefs.IsValid, null),

                ("log", ResourceDefs.Log, null),

                ("boundsRadius", GetBoundsRadius, null),
                ("type", GetTypeOfMode, null)
            };
        }

        static int Create(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.Skip();

            if(!l_argReader.ReadString(out string l_path))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            var l_model = Model.Import(l_path);
            if(!l_model.IsValid)
                Engine.Core.Core.Instance.LogManager.Log(string.Format("Model '{0}' import error: {1}", l_path, l_model.Log));

            l_argReader.PushObject(l_model, ms_modelType);
            return 1;
        }

        static int GetBoundsRadius(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Model l_model))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_model.BoundsRadius);
            return 1;
        }

        static int GetTypeOfMode(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Model l_model))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushString(l_model.TypeOfModel.ToString());
            return 1;
        }
    }
}

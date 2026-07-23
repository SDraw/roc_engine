using LuaSharp.Lua;
using ROC.Engine.Objects.Resources;
using ROC.Module.Wrappers;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class TextureDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_textureType = typeof(Texture);
        static readonly Type ms_vector2Type = typeof(Vector2);

        static TextureDefs()
        {
            Definition.m_name = nameof(Texture);
            Definition.m_constructor = Create;

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isValid", ObjectDefs.IsValid, null),

                ("log", ResourceDefs.Log, null),

                ("size", GetSize, null),
            };
        }

        static int Create(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.Skip();

            if(!l_argReader.ReadString(out string l_path) || !l_argReader.ReadBoolean(out bool l_alpha) || !l_argReader.ReadEnum(out Texture.TextureFiltering l_filter))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            var l_texture = Texture.Import(l_path, l_alpha, l_filter);
            if((l_texture.Log != null) && (l_texture.Log.Length > 0))
                Engine.Core.Core.Instance.LogManager.Log(string.Format("Texture '{0}' import error: {1}, dummy texture loaded instead", l_path, l_texture.Log));

            l_argReader.PushObject(l_texture, ms_textureType);
            return 1;
        }

        static int GetSize(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Texture l_tex))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushObject(new Vector2(l_tex.Size), ms_vector2Type);
            return 1;
        }
    }
}

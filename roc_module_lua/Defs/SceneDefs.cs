using LuaSharp.Lua;
using ROC.Engine.Objects;
using ROC.Engine.Objects.Resources;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class SceneDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_sceneType = typeof(Scene);
        static readonly Type ms_shaderType = typeof(Shader);

        static SceneDefs()
        {
            Definition.m_name = nameof(Scene);
            Definition.m_constructor = Create;

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isValid", ObjectDefs.IsValid, null),

                ("shader", GetShader, SetShader)
            };

            Definition.m_instanceMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("Add", AddGameObject),
                ("Remove", RemoveGameObject)
            };
        }

        // Create
        static int Create(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.PushObject(Scene.Create(), ms_sceneType);
            return 1;
        }

        // Shader
        static int GetShader(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Scene l_scene))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            if(l_scene.Shader != null)
                l_argReader.PushObject(l_scene.Shader, ms_shaderType);
            else
                l_argReader.PushBoolean(false);
            return 1;
        }

        static int SetShader(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Scene l_scene))
                return 0;

            if(l_argReader.IsNextNil())
            {
                l_scene.Shader = null;
                return 0;
            }

            if(!l_argReader.ReadObject(out Shader l_shader))
                return 0;

            l_scene.Shader = l_shader;
            return 0;
        }

        // GameObjects
        static int AddGameObject(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Scene l_scene) || !l_argReader.ReadObject(out GameObject l_go))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_scene.AddGameObject(l_go);
            l_argReader.PushBoolean(true);
            return 1;
        }

        static int RemoveGameObject(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Scene l_scene) || !l_argReader.ReadObject(out GameObject l_go))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_scene.RemoveGameObject(l_go);
            l_argReader.PushBoolean(true);
            return 1;
        }
    }
}

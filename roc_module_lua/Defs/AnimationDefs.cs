using LuaSharp.Lua;
using ROC.Engine.Objects.Resources;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class AnimationDefs
    {
        static public readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_animationType = typeof(Animation);

        static AnimationDefs()
        {
            Definition.m_name = nameof(Animation);
            Definition.m_constructor = Create;

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isLoaded", ResourceDefs.IsLoaded, null),
                ("log", ResourceDefs.Log, null),

                ("bonesCount", GetBonesCount, null),
                ("duration", GetDuration, null),
                ("fps", GetFPS, null),
                ("frameDelta", GetFrameDelta, null),
                ("framesCount", GetFramesCount, null)
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

            l_reader.PushObject(Animation.Import(l_path), ms_animationType);
            return 1;
        }

        static int GetBonesCount(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Animation l_animation))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushInteger(l_animation.BonesCount);
            return 1;
        }

        static int GetDuration(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Animation l_animation))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushInteger(l_animation.Duration);
            return 1;
        }

        static int GetFPS(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Animation l_animation))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushInteger(l_animation.FPS);
            return 1;
        }

        static int GetFrameDelta(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Animation l_animation))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushInteger(l_animation.FrameDelta);
            return 1;
        }

        static int GetFramesCount(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Animation l_animation))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushInteger(l_animation.FramesCount);
            return 1;
        }
    }
}

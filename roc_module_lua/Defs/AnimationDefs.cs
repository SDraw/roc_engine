using LuaSharp.Lua;
using ROC.Engine.Objects.Resources;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class AnimationDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_animationType = typeof(Animation);

        static AnimationDefs()
        {
            Definition.m_name = nameof(Animation);
            Definition.m_constructor = Create;

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isValid", ObjectDefs.IsValid, null),
                ("log", ResourceDefs.Log, null),

                ("bonesCount", GetBonesCount, null),
                ("duration", GetDuration, null),
                ("fps", GetFPS, null),
                ("frameDelta", GetFrameDelta, null),
                ("framesCount", GetFramesCount, null)
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

            l_argReader.PushObject(Animation.Import(l_path), ms_animationType);
            return 1;
        }

        static int GetBonesCount(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Animation l_animation))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushInteger(l_animation.BonesCount);
            return 1;
        }

        static int GetDuration(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Animation l_animation))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushInteger(l_animation.Duration);
            return 1;
        }

        static int GetFPS(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Animation l_animation))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushInteger(l_animation.FPS);
            return 1;
        }

        static int GetFrameDelta(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Animation l_animation))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushInteger(l_animation.FrameDelta);
            return 1;
        }

        static int GetFramesCount(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Animation l_animation))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushInteger(l_animation.FramesCount);
            return 1;
        }
    }
}

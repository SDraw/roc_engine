using LuaSharp.Lua;
using ROC.Engine.Objects.Resources;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class AudioClipDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_audioClipType = typeof(AudioClip);

        static AudioClipDefs()
        {
            Definition.m_name = nameof(AudioClip);
            Definition.m_constructor = Create;

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isValid", ObjectDefs.IsValid, null),

                ("log", ResourceDefs.Log, null),

                ("sampleRate", GetSampleRate, null),
                ("channels", GetChannels, null),
                ("duration", GetDuration, null),
            };
        }

        static int Create(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.Skip();

            if(!l_argReader.ReadString(out string l_val))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            AudioClip l_clip = AudioClip.Import(l_val);
            l_argReader.PushObject(l_clip, ms_audioClipType);
            return 1;
        }

        static int GetSampleRate(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioClip l_clip))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushInteger(l_clip.SampleRate);
            return 1;
        }

        static int GetChannels(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioClip l_clip))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushInteger(l_clip.Channels);
            return 1;
        }

        static int GetDuration(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioClip l_clip))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_clip.Duration);
            return 1;
        }
    }
}

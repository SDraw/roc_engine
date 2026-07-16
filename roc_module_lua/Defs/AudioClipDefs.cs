using LuaSharp.Lua;
using ROC.Engine.Objects.Resources;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class AudioClipDefs
    {
        static public readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_audioClipType = typeof(AudioClip);

        static AudioClipDefs()
        {
            Definition.m_name = nameof(AudioClip);
            Definition.m_constructor = Create;

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),

                ("isLoaded", ResourceDefs.IsLoaded, null),
                ("log", ResourceDefs.Log, null),

                ("sampleRate", GetSampleRate, null),
                ("channels", GetChannels, null),
                ("duration", GetDuration, null),
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

            if(!l_reader.ReadString(out string l_val))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            AudioClip l_clip = AudioClip.Import(l_val);
            l_reader.PushObject(l_clip, ms_audioClipType);
            return 1;
        }

        static int GetSampleRate(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out AudioClip l_clip))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushInteger(l_clip.SampleRate);
            return 1;
        }

        static int GetChannels(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out AudioClip l_clip))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushInteger(l_clip.Channels);
            return 1;
        }

        static int GetDuration(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out AudioClip l_clip))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushNumber(l_clip.Duration);
            return 1;
        }
    }
}

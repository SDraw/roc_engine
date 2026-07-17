using LuaSharp.Lua;
using ROC.Engine.Objects.Components;
using ROC.Engine.Objects.Resources;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class AudioSourceDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_audioClipType = typeof(AudioClip);

        static AudioSourceDefs()
        {
            Definition.m_name = nameof(AudioSource);

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isValid", ObjectDefs.IsValid, null),

                ("type", ComponentDefs.GetComponentType, null),
                ("priority", ComponentDefs.GetPriority, null),
                ("enabled", ComponentDefs.GetEnabled, ComponentDefs.SetEnabled),
                ("gameObject", ComponentDefs.GetGameObject, null),

                ("clip", GetClip, SetClip),
                ("time", GetTime, SetTime),
                ("volume", GetVolume, SetVolume),
                ("pitch", GetPitch, SetPitch),
                ("loop", GetLoop, SetLoop),
                ("minDistance", GetMinDistance, SetMinDistance),
                ("attenuation", GetAttenuation, SetAttenuation),
                ("status", GetStatus, null)
            };

            Definition.m_instanceMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("Play", Play),
                ("Pause", Pause),
                ("Stop", Stop)
            };
        }

        static int GetClip(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioSource l_source))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            if(l_source.AudioClip != null)
                l_argReader.PushObject(l_source.AudioClip, ms_audioClipType);
            else
                l_argReader.PushBoolean(false);
            return 1;
        }
        static int SetClip(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioSource l_source))
                return 0;

            if(l_argReader.IsNextObject())
            {
                if(!l_argReader.ReadObject(out AudioClip l_clip))
                    return 0;

                l_source.AudioClip = l_clip;
                return 0;
            }

            if(l_argReader.IsNextNil())
            {
                l_source.AudioClip = null;
                return 0;
            }

            return 0;
        }

        static int GetTime(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioSource l_source))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_source.Time);
            return 1;
        }
        static int SetTime(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioSource l_source) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_source.Time = l_val;
            return 0;
        }

        static int GetVolume(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioSource l_source))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_source.Volume);
            return 1;
        }
        static int SetVolume(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioSource l_source) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_source.Volume = l_val;
            return 0;
        }

        static int GetPitch(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioSource l_source))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_source.Pitch);
            return 1;
        }
        static int SetPitch(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioSource l_source) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_source.Pitch = l_val;
            return 0;
        }

        static int GetLoop(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioSource l_source))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushBoolean(l_source.Loop);
            return 1;
        }
        static int SetLoop(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioSource l_source) || !l_argReader.ReadBoolean(out bool l_val))
                return 0;

            l_source.Loop = l_val;
            return 0;
        }

        static int GetMinDistance(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioSource l_source))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_source.MinDistance);
            return 1;
        }
        static int SetMinDistance(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioSource l_source) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_source.MinDistance = l_val;
            return 0;
        }

        static int GetAttenuation(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioSource l_source))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_source.Attenuation);
            return 1;
        }
        static int SetAttenuation(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioSource l_source) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_source.Attenuation = l_val;
            return 0;
        }

        static int GetStatus(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioSource l_source))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushString(l_source.Status.ToString());
            return 1;
        }

        static int Play(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioSource l_source))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_source.Play();
            l_argReader.PushBoolean(true);
            return 1;
        }

        static int Pause(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioSource l_source))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_source.Pause();
            l_argReader.PushBoolean(true);
            return 1;
        }

        static int Stop(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out AudioSource l_source))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_source.Stop();
            l_argReader.PushBoolean(true);
            return 1;
        }
    }
}

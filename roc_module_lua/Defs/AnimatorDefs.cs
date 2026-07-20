using LuaSharp.Lua;
using ROC.Engine.Objects.Components;
using ROC.Engine.Objects.Resources;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class AnimatorDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_animationType = typeof(Animation);

        static AnimatorDefs()
        {
            Definition.m_name = nameof(Animator);

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),
                ("isValid", ObjectDefs.IsValid, null),

                ("type", ComponentDefs.GetComponentType, null),
                ("priority", ComponentDefs.GetPriority, null),
                ("enabled", ComponentDefs.GetEnabled, ComponentDefs.SetEnabled),
                ("gameObject", ComponentDefs.GetGameObject, null),

                ("animation", GetAnimation, SetAnimation),
                ("speed", GetSpeed, SetSpeed),
                ("loop", GetLoop, SetLoop)
            };
        }

        static int GetAnimation(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Animator l_animator))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            if(l_animator.Animation != null)
                l_argReader.PushObject(l_animator.Animation, ms_animationType);
            else
                l_argReader.PushBoolean(false);
            return 1;
        }

        static int SetAnimation(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Animator l_animator))
                return 0;

            if(l_argReader.IsNextObject())
            {
                if(!l_argReader.ReadObject(out Animation l_anim))
                    return 0;

                l_animator.Animation = l_anim;
                return 0;
            }

            if(l_argReader.IsNextNil())
            {
                l_animator.Animation = null;
                return 0;
            }

            return 0;
        }

        static int GetSpeed(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Animator l_animator))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushNumber(l_animator.Speed);
            return 1;
        }

        static int SetSpeed(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Animator l_animator) || !l_argReader.ReadNumber(out float l_val))
                return 0;

            l_animator.Speed = l_val;
            return 0;
        }

        static int GetLoop(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Animator l_animator))
            {
                l_argReader.PushBoolean(false);
                return 1;
            }

            l_argReader.PushBoolean(l_animator.Loop);
            return 1;
        }

        static int SetLoop(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Animator l_animator) || !l_argReader.ReadBoolean(out bool l_val))
                return 0;

            l_animator.Loop = l_val;
            return 0;
        }
    }
}

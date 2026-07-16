using LuaSharp.Lua;
using ROC.Engine.Objects.Components;
using ROC.Engine.Objects.Resources;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class AnimatorDefs
    {
        static public readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_animationType = typeof(Animation);

        static AnimatorDefs()
        {
            Definition.m_name = nameof(Animator);

            Definition.m_instanceProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("instanceID", ObjectDefs.GetInstanceID, null),

                ("isValid", ComponentDefs.IsValid, null),
                ("type", ComponentDefs.GetComponentType, null),
                ("priority", ComponentDefs.GetPriority, null),
                ("enabled", ComponentDefs.GetEnabled, ComponentDefs.SetEnabled),
                ("gameObject", ComponentDefs.GetGameObject, null),

                ("animation", GetAnimation, SetAnimation),
                ("speed", GetSpeed, SetSpeed)
            };

            Definition.m_instanceMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("Destroy", ComponentDefs.Destroy)
            };
        }

        static int GetAnimation(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Animator l_animator))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            if(l_animator.Animation != null)
                l_reader.PushObject(l_animator.Animation, ms_animationType);
            else
                l_reader.PushBoolean(false);
            return 1;
        }

        static int SetAnimation(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Animator l_animator))
                return 0;

            if(l_reader.IsNextObject())
            {
                if(!l_reader.ReadObject(out Animation l_anim))
                    return 0;

                l_animator.Animation = l_anim;
                return 0;
            }

            if(l_reader.IsNextNil())
            {
                l_animator.Animation = null;
                return 0;
            }

            return 0;
        }

        static int GetSpeed(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Animator l_animator))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushNumber(l_animator.Speed);
            return 1;
        }

        static int SetSpeed(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadObject(out Animator l_animator) || !l_reader.ReadNumber(out float l_val))
                return 0;

            l_animator.Speed = l_val;
            return 0;
        }
    }
}

using GlmSharp;
using LuaSharp.Lua;
using ROC.Module.Wrappers;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class WindowDefs
    {
        public static readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();
        static readonly Type ms_vector2Type = typeof(Vector2);

        static WindowDefs()
        {
            Definition.m_name = "Window";

            Definition.m_staticProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("position", GetPosition, SetPosition),
                ("size", GetSize, SetSize),
                ("vsync", null, SetVSync),
                ("frameLimit", null, SetFrameLimit),
                ("title", null, SetTitle),
                ("icon", null, SetIcon),
                ("focus", GetFocus, null),
                ("cursorGrabbed", null, SetCursorGrab),
                ("cursorVisible", null, SetCursorVisible),
                ("mousePosition", GetMousePosition, SetMousePosition)
            };

            Definition.m_staticMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("Close", Close),
                ("RequestFocus", RequestFocus)
            };
        }

        static int GetPosition(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.PushObject(new Vector2(Engine.Core.Core.Instance.WindowManager.WindowPosition), ms_vector2Type);
            return 1;
        }
        static int SetPosition(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Vector2 p_val))
                return 0;

            Engine.Core.Core.Instance.WindowManager.WindowPosition = (GlmSharp.ivec2)p_val.m_vector;
            return 0;
        }

        static int GetSize(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.PushObject(new Vector2(Engine.Core.Core.Instance.WindowManager.WindowSize), ms_vector2Type);
            return 1;
        }
        static int SetSize(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Vector2 p_val))
                return 0;

            Engine.Core.Core.Instance.WindowManager.WindowSize = (GlmSharp.uvec2)p_val.m_vector;
            return 0;
        }

        static int SetVSync(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadBoolean(out bool p_val))
                return 0;

            Engine.Core.Core.Instance.WindowManager.SetVSync(p_val);
            return 0;
        }

        static int SetFrameLimit(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadInteger(out int p_val))
                return 0;

            Engine.Core.Core.Instance.WindowManager.SetFrameLmit((uint)p_val);
            return 0;
        }

        static int SetTitle(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadString(out string p_val))
                return 0;

            Engine.Core.Core.Instance.WindowManager.SetTitle(p_val);
            return 0;
        }

        static int SetIcon(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadString(out string p_val))
                return 0;

            Engine.Core.Core.Instance.WindowManager.SetIcon(p_val);
            return 0;
        }

        static int GetFocus(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.PushBoolean(Engine.Core.Core.Instance.WindowManager.Focus);
            return 1;
        }

        static int RequestFocus(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            Engine.Core.Core.Instance.WindowManager.RequestFocus();
            l_argReader.PushBoolean(true);
            return 1;
        }

        static int SetCursorGrab(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadBoolean(out bool p_val))
                return 0;

            Engine.Core.Core.Instance.WindowManager.SetCursorGrabbed(p_val);
            return 0;
        }

        static int SetCursorVisible(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadBoolean(out bool p_val))
                return 0;

            Engine.Core.Core.Instance.WindowManager.SetCursorVisible(p_val);
            return 0;
        }

        static int Close(IntPtr p_state)
        {
            Engine.Core.Core.Instance.WindowManager.CloseWindow();
            return 0;
        }

        static int GetMousePosition(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            l_argReader.PushObject(new Vector2(Engine.Core.Core.Instance.WindowManager.GetMousePosition()), ms_vector2Type);
            return 1;
        }
        static int SetMousePosition(IntPtr p_state)
        {
            var l_argReader = new ArgReader(p_state);
            if(!l_argReader.ReadObject(out Vector2 l_pos))
                return 0;

            Engine.Core.Core.Instance.WindowManager.SetMousePosition((ivec2)l_pos.m_vector);
            return 0;
        }
    }
}

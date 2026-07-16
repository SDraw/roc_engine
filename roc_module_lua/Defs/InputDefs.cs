using LuaSharp.Lua;
using ROC.Engine.Managers;
using SFML.Window;
using System;
using System.Collections.Generic;

namespace ROC.Module.Defs
{
    internal static class InputDefs
    {
        static public readonly LuaVM.LuaClassDefinition Definition = new LuaVM.LuaClassDefinition();

        static InputDefs()
        {
            Definition.m_name = "Input";

            Definition.m_staticProperties = new List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)>()
            {
                ("clipboard", GetClipboard, SetClipboard)
            };

            Definition.m_staticMethods = new List<(string, LuaInterop.lua_CFunction)>()
            {
                ("GetKey", GetKey),
                ("GetMouseButton", GetMouseButton),
                ("IsControllerConnected", IsControllerConnected),
                ("GetControllerButtonsCount", GetControllerButtonsCount),
                ("GetControllerButton", GetControllerButton),
                ("IsControllerAxisPresent", IsControllerAxisPresent),
                ("GetControllerAxis", GetControllerAxis)
            };
        }

        static int GetClipboard(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            l_reader.PushString(InputManager.Clipboard);
            return 1;
        }
        static int SetClipboard(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadString(out string l_val))
                return 0;

            InputManager.Clipboard = l_val;
            return 0;
        }

        static int GetKey(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadEnum(out Keyboard.Key l_key))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushBoolean(InputManager.GetKey(l_key));
            return 1;
        }

        static int GetMouseButton(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadEnum(out Mouse.Button l_button))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_reader.PushBoolean(InputManager.GetMouseButton(l_button));
            return 1;
        }

        static int IsControllerConnected(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadInteger(out int l_index))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_index = Math.Max(0, l_index);
            l_reader.PushBoolean(InputManager.IsControllerConnected((uint)l_index));
            return 1;
        }

        static int GetControllerButtonsCount(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadInteger(out int l_index))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_index = Math.Max(0, l_index);
            l_reader.PushInteger(InputManager.GetControllerButtonsCount((uint)l_index));
            return 1;
        }

        static int GetControllerButton(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadInteger(out int l_index) || !l_reader.ReadInteger(out int l_button))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_index = Math.Max(0, l_index);
            l_button = Math.Max(0, l_button);
            l_reader.PushBoolean(InputManager.GetControllerButton((uint)l_index, (uint)l_button));
            return 1;
        }

        static int IsControllerAxisPresent(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadInteger(out int l_index) || !l_reader.ReadEnum(out Joystick.Axis l_axis))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_index = Math.Max(0, l_index);
            l_reader.PushBoolean(InputManager.IsControllerAxisPresent((uint)l_index, l_axis));
            return 1;
        }

        static int GetControllerAxis(IntPtr p_state)
        {
            ArgReader l_reader = new ArgReader(p_state);
            if(!l_reader.ReadInteger(out int l_index) || !l_reader.ReadEnum(out Joystick.Axis l_axis))
            {
                l_reader.PushBoolean(false);
                return 1;
            }

            l_index = Math.Max(0, l_index);
            l_reader.PushNumber(InputManager.GetControllerAxis((uint)l_index, l_axis));
            return 1;
        }
    }
}

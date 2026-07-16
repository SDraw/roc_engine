using SFML.Window;

namespace ROC.Engine.Managers
{
    public sealed class InputManager : Manager
    {
        internal InputManager()
        {
        }

        public static string Clipboard
        {
            get => SFML.Window.Clipboard.Contents;
            set => SFML.Window.Clipboard.Contents = value;
        }

        public static bool GetKey(Keyboard.Key p_key) => Keyboard.IsKeyPressed(p_key);

        public static bool GetMouseButton(Mouse.Button p_button) => Mouse.IsButtonPressed(p_button);

        public static bool IsControllerConnected(uint p_controller) => Joystick.IsConnected(p_controller);

        public static bool GetControllerButton(uint p_controller, uint p_button) => Joystick.IsButtonPressed(p_controller, p_button);

        public static uint GetControllerButtonsCount(uint p_controller) => Joystick.GetButtonCount(p_controller);

        public static bool IsControllerAxisPresent(uint p_controller, Joystick.Axis p_axis) => Joystick.HasAxis(p_controller, p_axis);

        public static float GetControllerAxis(uint p_controller, Joystick.Axis p_axis) => Joystick.GetAxisPosition(p_controller, p_axis);
    }
}

using GlmSharp;
using OpenGL;
using SFML.Graphics;
using SFML.System;
using SFML.Window;
using System;

namespace ROC.Engine.Managers
{

    public sealed class WindowManager : Manager
    {
        ContextSettings m_contextSettings;
        VideoMode m_videoMode;
        RenderWindow m_window = null;

        uvec2 m_windowSize = uvec2.Ones;
        bool m_closeWindow = false;

        readonly Clock m_clock = null;
        float m_time = 0f;

        internal WindowManager()
        {
            m_contextSettings = new ContextSettings();
            m_contextSettings.DepthBits = 24U;
            m_contextSettings.StencilBits = 8U;
            m_contextSettings.SRgbCapable = false;
            m_contextSettings.AttributeFlags = ContextSettings.Attribute.Core;

            m_videoMode = new VideoMode();

            m_clock = new Clock();
            m_time = 0f;
        }

        internal override void Start()
        {
            if(m_active)
                return;

            Gl.Initialize();

            m_windowSize = Core.Core.Instance.ConfigManager.WindowSize;

            m_videoMode.Width = m_windowSize.x;
            m_videoMode.Height = m_windowSize.y;

            m_contextSettings.AntialiasingLevel = Core.Core.Instance.ConfigManager.AntialiasingLevel;

            Styles l_style = Core.Core.Instance.ConfigManager.IsFullscreen ? (Styles.Default | Styles.Fullscreen) : Styles.Default;
            m_window = new RenderWindow(m_videoMode, "ROC Engine Application", l_style, m_contextSettings);
            m_window.SetActive(true);

            Gl.BindAPI();

            var l_contextSettings = m_window.Settings;
            if(l_contextSettings.MajorVersion * 10U + l_contextSettings.MinorVersion < 31U)
            {
                string l_error = string.Format("Minimal supported OpenGL version - 3.1\nSystem OpenGL version - {0}.{1}", l_contextSettings.MajorVersion, l_contextSettings.MinorVersion);
                Core.Core.Instance.LogManager.Log(l_error);
                Environment.Exit(-1);
                return;
            }

            Core.Core.Instance.LogManager.Log(
                string.Format("{0}, {1}, {2}, {3}",
                    Gl.GetString(StringName.Vendor),
                    Gl.GetString(StringName.Renderer),
                    Gl.GetString(StringName.Version),
                    Gl.GetString(StringName.ShadingLanguageVersion)
                )
            );

            m_active = true;

            m_window.SetFramerateLimit(Core.Core.Instance.ConfigManager.FpsLimit);
            m_window.SetVerticalSyncEnabled(Core.Core.Instance.ConfigManager.VSync);
            m_window.SetKeyRepeatEnabled(false);
            SetIcon("roc_icon.png");

            m_window.Closed += this.OnClose;
            m_window.Resized += this.OnResize;
            m_window.GainedFocus += this.OnGainedFocus;
            m_window.LostFocus += this.OnLostFocus;
            m_window.KeyPressed += this.OnKeyPressed;
            m_window.KeyReleased += this.OnKeyReleased;
            m_window.TextEntered += this.OnTextEntered;
            m_window.MouseMoved += this.OnMouseMoved;
            m_window.MouseEntered += this.OnMouseEntered;
            m_window.MouseLeft += this.OnMouseLeft;
            m_window.MouseButtonPressed += this.OnMouseButtonPressed;
            m_window.MouseButtonReleased += this.OnMouseButtonReleased;
            m_window.MouseWheelScrolled += this.OnMouseScroll;
            m_window.JoystickConnected += this.OnControllerConnected;
            m_window.JoystickDisconnected += this.OnControllerDisconnected;
            m_window.JoystickButtonPressed += this.OnControllerButtonPressed;
            m_window.JoystickButtonReleased += this.OnControllerButtonReleased;
            m_window.JoystickMoved += this.OnControllerAxis;

            m_clock.Restart();
        }

        internal override void Stop()
        {
            if(!m_active)
                return;

            m_window.SetActive(false);
            m_window.Close();
            m_window.Dispose();
            m_window = null;

            m_active = false;
        }

        // Updates
        internal bool DoPulse_S1()
        {
            m_time = m_clock.ElapsedTime.AsSeconds();
            m_window.DispatchEvents();
            return (m_active && !m_closeWindow);
        }

        internal bool DoPulse_S2()
        {
            m_window.Display();
            return (m_active && !m_closeWindow);
        }

        // Events
        void OnClose(object p_sender, EventArgs p_args)
        {
            Core.Core.Instance.ModuleManager.SendEvent(Modules.ModuleEvent.OnWindowClose);
            m_closeWindow = true;
        }

        void OnResize(object p_sender, SizeEventArgs p_args)
        {
            m_windowSize.x = p_args.Width;
            m_windowSize.y = p_args.Height;

            Core.Core.Instance.ModuleManager.SendEvent(Modules.ModuleEvent.OnWindowResize, p_args.Width, p_args.Height);
        }

        void OnGainedFocus(object p_sender, EventArgs p_args)
        {
            Core.Core.Instance.ModuleManager.SendEvent(Modules.ModuleEvent.OnWindowFocus, true);
        }

        void OnLostFocus(object p_sender, EventArgs p_args)
        {
            Core.Core.Instance.ModuleManager.SendEvent(Modules.ModuleEvent.OnWindowFocus, false);
        }

        void OnKeyPressed(object p_sender, SFML.Window.KeyEventArgs p_args)
        {
            Core.Core.Instance.ModuleManager.SendEvent(Modules.ModuleEvent.OnKey, p_args.Scancode.ToString(), true);
        }

        void OnKeyReleased(object p_sender, SFML.Window.KeyEventArgs p_args)
        {
            Core.Core.Instance.ModuleManager.SendEvent(Modules.ModuleEvent.OnKey, p_args.Scancode.ToString(), false);
        }

        void OnTextEntered(object p_sender, TextEventArgs p_args)
        {
            Core.Core.Instance.ModuleManager.SendEvent(Modules.ModuleEvent.OnText, p_args.Unicode);
        }

        void OnMouseMoved(object p_sender, MouseMoveEventArgs p_args)
        {
            Core.Core.Instance.ModuleManager.SendEvent(Modules.ModuleEvent.OnMouseMove, p_args.X, p_args.Y);
        }

        void OnMouseEntered(object p_sender, EventArgs p_args)
        {
            Core.Core.Instance.ModuleManager.SendEvent(Modules.ModuleEvent.OnMouseFocus, true);
        }

        void OnMouseLeft(object p_sender, EventArgs p_args)
        {
            Core.Core.Instance.ModuleManager.SendEvent(Modules.ModuleEvent.OnMouseFocus, false);
        }

        void OnMouseButtonPressed(object p_sender, MouseButtonEventArgs p_args)
        {
            Core.Core.Instance.ModuleManager.SendEvent(Modules.ModuleEvent.OnMouseKey, p_args.Button.ToString(), true);
        }

        void OnMouseButtonReleased(object p_sender, MouseButtonEventArgs p_args)
        {
            Core.Core.Instance.ModuleManager.SendEvent(Modules.ModuleEvent.OnMouseKey, p_args.Button.ToString(), false);
        }

        void OnMouseScroll(object p_sender, MouseWheelScrollEventArgs p_args)
        {
            Core.Core.Instance.ModuleManager.SendEvent(Modules.ModuleEvent.OnMouseScroll, p_args.Delta, p_args.Wheel.ToString());
        }

        void OnControllerConnected(object p_sender, JoystickConnectEventArgs p_args)
        {
            Core.Core.Instance.ModuleManager.SendEvent(Modules.ModuleEvent.OnControllerConnection, p_args.JoystickId, true);
        }

        void OnControllerDisconnected(object p_sender, JoystickConnectEventArgs p_args)
        {
            Core.Core.Instance.ModuleManager.SendEvent(Modules.ModuleEvent.OnControllerConnection, p_args.JoystickId, false);
        }

        void OnControllerButtonPressed(object p_sender, JoystickButtonEventArgs p_args)
        {
            Core.Core.Instance.ModuleManager.SendEvent(Modules.ModuleEvent.OnControllerButton, p_args.JoystickId, p_args.Button, true);
        }

        void OnControllerButtonReleased(object p_sender, JoystickButtonEventArgs p_args)
        {
            Core.Core.Instance.ModuleManager.SendEvent(Modules.ModuleEvent.OnControllerButton, p_args.JoystickId, p_args.Button, false);
        }

        void OnControllerAxis(object p_sender, JoystickMoveEventArgs p_args)
        {
            Core.Core.Instance.ModuleManager.SendEvent(Modules.ModuleEvent.OnControllerAxis, p_args.JoystickId, p_args.Axis.ToString(), p_args.Position);
        }

        // Arbitrary
        public uvec2 WindowSize
        {
            get => m_windowSize;
            set
            {
                if(m_window == null)
                    return;

                m_windowSize = value;
                m_window.Size = new Vector2u(m_windowSize.x, m_windowSize.y);
            }
        }

        public ivec2 WindowPosition
        {
            get => (m_window != null) ? new ivec2(m_window.Position.X, m_window.Position.Y) : ivec2.Zero;
            set
            {
                if(m_window == null)
                    return;

                m_window.Position = new Vector2i(value.x, value.y);
            }
        }

        public float Time => m_time;

        public void SetIcon(string p_path)
        {
            if(!m_active)
                return;

            try
            {
                Image l_icon = new Image(p_path);
                m_window.SetIcon(l_icon.Size.X, l_icon.Size.Y, l_icon.Pixels);
            }
            catch(Exception e)
            {
                Core.Core.Instance.LogManager.Log(e.Message);
            }
        }

        public void SetVSync(bool p_state) => m_window?.SetVerticalSyncEnabled(p_state);

        public void SetFrameLmit(uint p_value) => m_window?.SetFramerateLimit(p_value);

        public void SetTitle(string p_title) => m_window?.SetTitle(p_title);

        public bool Focus => (m_window != null) && m_window.HasFocus();

        public void RequestFocus() => m_window?.RequestFocus();

        public void SetCursorGrabbed(bool p_state) => m_window?.SetMouseCursorGrabbed(p_state);

        public void SetCursorVisible(bool p_state) => m_window?.SetMouseCursorVisible(p_state);

        public void CloseWindow() => m_active = false;

        public ivec2 GetMousePosition()
        {
            if(m_window == null)
                return ivec2.Zero;

            var l_pos = Mouse.GetPosition(m_window);
            return new ivec2(l_pos.X, l_pos.Y);
        }

        public void SetMousePosition(ivec2 p_pos)
        {
            if(m_window == null)
                return;

            Mouse.SetPosition(new Vector2i(p_pos.x, p_pos.y), m_window);
        }
    }
}

namespace ROC.Modules
{
    public enum ModuleEvent : int
    {
        OnEngineStart = 0,
        OnEngineStop,

        OnUpdate,
        OnRender,

        OnWindowClose,
        OnWindowResize,
        OnWindowFocus,

        OnKey,
        OnText,

        OnMouseMove,
        OnMouseFocus,
        OnMouseKey,
        OnMouseScroll,

        OnControllerConnection,
        OnControllerButton,
        OnControllerAxis,
    }

    public interface IModule
    {
        void OnEvent(ModuleEvent p_event);
        void OnEvent(ModuleEvent p_event, params object[] p_args);
    }
}

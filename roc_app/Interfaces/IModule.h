#pragma once

namespace ROC
{

class ICustomArguments;

class IModule
{
public:
    enum ModuleEvent : unsigned char
    {
        ME_OnEngineStart = 0U,
        ME_OnEngineStop,
        ME_OnRender,
        ME_OnVRRender,
        ME_OnPreRender,
        ME_OnWindowClose,
        ME_OnWindowResize,
        ME_OnWindowFocus,
        ME_OnKeyPress,
        ME_OnMouseKeyPress,
        ME_OnMouseScroll,
        ME_OnCursorMove,
        ME_OnCursorEnter,
        ME_OnJoypadStateChange,
        ME_OnJoypadButton,
        ME_OnJoypadAxis,
        ME_OnTextInput
    };

    virtual void RecieveGlobalEvent(unsigned char p_event, const ICustomArguments *p_args) = 0;
    virtual void DoPulse() = 0;
};

}

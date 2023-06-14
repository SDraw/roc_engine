#pragma once

namespace ROC
{

class ISfmlManager
{
public:
    virtual void GetWindowPosition(glm::ivec2 &p_pos) const = 0;
    virtual void SetWindowPosition(const glm::ivec2 &p_pos) = 0;
    virtual void GetWindowSize(glm::ivec2 &p_size) const = 0;
    virtual void CloseWindow() = 0;
    virtual void SetVSync(bool p_sync) = 0;
    virtual void SetFramelimit(unsigned int p_fps) = 0;
    virtual unsigned int GetFramelimit() const = 0;
    virtual void SetTitle(const std::string &p_title) = 0;
    virtual bool SetIcon(const std::string &p_path) = 0;
    virtual void RequestFocus() = 0;
    virtual bool GetFocusState() const = 0;
    virtual void SetInputEnabled(bool p_state) = 0;
    virtual void SetCursorMode(bool p_visible, bool p_lock) = 0;
    virtual void GetCursorPosition(glm::ivec2 &p_pos) const = 0;
    virtual void SetCursorPosition(const glm::ivec2 &p_pos) = 0;
    virtual void GetClipboardString(std::string &p_str) const = 0;
    virtual void SetClipboardString(const std::string &p_str) = 0;
    virtual bool IsKeyPressed(int p_key) = 0;
    virtual bool IsMouseKeyPressed(int p_key) = 0;
    virtual bool IsJoypadConnected(unsigned int p_jp) = 0;
    virtual bool GetJoypadButtonState(unsigned int p_jp, unsigned int p_button) = 0;
    virtual unsigned int GetJoypadButtonCount(unsigned int p_jp) = 0;
    virtual bool CheckJoypadAxis(unsigned int p_jp, unsigned int p_axis) = 0;
    virtual float GetJoypadAxisValue(unsigned int p_jp, unsigned int p_axis) = 0;
    virtual float GetTime() const = 0;
};

}

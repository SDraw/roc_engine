#pragma once
#include "Interfaces/ISfmlManager.h"
#include "Managers/Manager.h"

namespace ROC
{

class CustomArguments;

class SfmlManager final : public ISfmlManager, public Manager
{
    sf::VideoMode m_windowVideoMode;
    sf::ContextSettings m_contextSettings;
    sf::Uint32 m_windowStyle;
    unsigned int m_frameLimit;
    sf::Window *m_window;
    sf::Event m_event;
    bool m_active;

    sf::Clock m_clock;
    float m_time;

    bool m_inputEnabled;

    CustomArguments *m_arguments;

    SfmlManager(const SfmlManager &that) = delete;
    SfmlManager& operator=(const SfmlManager &that) = delete;
public:
    explicit SfmlManager(Core *p_core);
    ~SfmlManager();

    // ROC::Manager
    void Start() override;
    void Stop() override;

    void GetWindowPosition(glm::ivec2 &p_pos) const;
    void SetWindowPosition(const glm::ivec2 &p_pos);
    void GetWindowSize(glm::ivec2 &p_size) const;
    void CloseWindow();
    void SetVSync(bool p_sync);
    void SetFramelimit(unsigned int p_fps);
    unsigned int GetFramelimit() const;
    void SetTitle(const std::string &p_title);
    bool SetIcon(const std::string &p_path);
    void RequestFocus();
    bool GetFocusState() const;
    void SetInputEnabled(bool p_state);

    void SetCursorMode(bool p_visible, bool p_lock);
    void GetCursorPosition(glm::ivec2 &p_pos) const;
    void SetCursorPosition(const glm::ivec2 &p_pos);

    void GetClipboardString(std::string &p_str) const;
    void SetClipboardString(const std::string &p_str);

    bool IsKeyPressed(int p_key);
    bool IsMouseKeyPressed(int p_key);

    bool IsJoypadConnected(unsigned int p_jp);
    bool GetJoypadButtonState(unsigned int p_jp, unsigned int p_button);
    unsigned int GetJoypadButtonCount(unsigned int p_jp);
    bool CheckJoypadAxis(unsigned int p_jp, unsigned int p_axis);
    float GetJoypadAxisValue(unsigned int p_jp, unsigned int p_axis);

    float GetTime() const;

    void UpdateWindow();

    bool DoPulse();
};

}

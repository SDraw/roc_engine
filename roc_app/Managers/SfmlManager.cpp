#include "stdafx.h"

#include "Managers/SfmlManager.h"
#include "Core/Core.h"
#include "Utils/CustomArguments.h"

#include "Managers/ConfigManager.h"
#include "Managers/LogManager.h"
#include "Managers/ModuleManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Interfaces/IModule.h"
#include "GL/GLSetting.h"
#include "GL/GLState.h"

SFML_DEFINE_DISCRETE_GPU_PREFERENCE

namespace ROC
{

const size_t g_glMinimalVersion = 31U;

const std::string g_defaultIconPath("roc_icon.png");

const std::array<std::string, 101U> g_keyNames =
{
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
    "esc", "lctrl", "lshift", "lalt", "lsys", "rctrl", "rshift", "ralt", "rsys", "menu",
    "lbracket", "rbracket", "semicolon", "comma", "period", "quote", "slash", "backslash", "tilde", "equal", "dash",
    "space", "return", "backspace", "tab", "pgup", "pgdn", "end", "home", "insert", "delete",
    "num+", "num-", "num*", "num/",
    "arrow_l", "arrow_r", "arrow_u", "arrow_d",
    "num0", "num1", "num2", "num3", "num4", "num5", "num6", "num7", "num8", "num9",
    "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15",
    "pause"
};
const std::array<std::string, 5U> g_mouseKeyNames =
{
    "left", "right", "middle",
    "x1", "x2"
};
const std::array<std::string, 8U> g_joypadAxisNames =
{
    "x", "y", "z",
    "r", "u", "v",
    "povx", "povy"
};

}

ROC::SfmlManager::SfmlManager(Core *p_core) : Manager(p_core)
{
    m_time = 0.f;
    m_active = false;
    m_frameLimit = 60U;
    m_inputEnabled = false;
    m_arguments = new CustomArguments();
    std::memset(&m_event, 0, sizeof(sf::Event));
    m_title.assign("RunOnCoal Engine Application");
}

ROC::SfmlManager::~SfmlManager()
{
    delete m_arguments;
}

void ROC::SfmlManager::Start()
{
    if(!IsActive())
    {
        const ConfigManager *l_configManager = GetCore()->GetConfigManager();
        const glm::ivec2 &l_windowSize = l_configManager->GetWindowSize();
        m_windowVideoMode = sf::VideoMode(l_windowSize.x, l_windowSize.y);

        m_contextSettings.antialiasingLevel = static_cast<unsigned int>(l_configManager->GetAntialiasing());
        m_contextSettings.depthBits = 24U;
#ifdef _DEBUG
        m_contextSettings.attributeFlags = (sf::ContextSettings::Attribute::Core | sf::ContextSettings::Attribute::Debug);
#else
        m_contextSettings.attributeFlags = sf::ContextSettings::Attribute::Core;
#endif
        m_contextSettings.majorVersion = 0U;
        m_contextSettings.minorVersion = 0U;
        m_contextSettings.sRgbCapable = false;
        m_contextSettings.stencilBits = 8U;

        m_windowStyle = (l_configManager->IsFullscreenEnabled() ? sf::Style::Fullscreen : sf::Style::Default);

        m_window = new sf::Window(m_windowVideoMode, m_title, m_windowStyle, m_contextSettings);
        m_window->setActive(true);
        m_active = true;

        std::string l_log;
        if(GLSetting::GetString(GL_VERSION) == NULL)
        {
            l_log.assign("SFML: Unable to create OpenGL ");
            l_log.append(std::to_string(m_contextSettings.majorVersion));
            l_log.push_back('.');
            l_log.append(std::to_string(m_contextSettings.minorVersion));
            l_log.append(" core context. Check GPU for OpenGL support.");
            GetCore()->GetLogManager()->Log(l_log.c_str());

            MessageBoxA(m_window->getSystemHandle(), l_log.c_str(), NULL, MB_OK | MB_ICONEXCLAMATION);
            std::exit(-1);
        }
        else
        {
            sf::ContextSettings l_createdContextSettings = m_window->getSettings();
            if(l_createdContextSettings.majorVersion * 10U + l_createdContextSettings.minorVersion < g_glMinimalVersion)
            {
                l_log.assign("SFML: Minimal supported OpenGL version - 3.1\nSystem OpenGL version - ");
                l_log.append(std::to_string(l_createdContextSettings.majorVersion));
                l_log.push_back('.');
                l_log.append(std::to_string(l_createdContextSettings.minorVersion));
                GetCore()->GetLogManager()->Log(l_log.c_str());

                MessageBoxA(m_window->getSystemHandle(), l_log.c_str(), NULL, MB_OK | MB_ICONEXCLAMATION);
                std::exit(-1);
            }
        }
        m_frameLimit = l_configManager->GetFPSLimit();
        m_window->setFramerateLimit(m_frameLimit);
        m_window->setVerticalSyncEnabled(l_configManager->GetVSync());
        m_window->setKeyRepeatEnabled(false);
        SetIcon(g_defaultIconPath);

        GLenum l_error = glewInit();
        if(l_error != GLEW_OK)
        {
            l_log.assign("GLEW: ");
            l_log.append(reinterpret_cast<const char*>(glewGetErrorString(l_error)));
            GetCore()->GetLogManager()->Log(l_log.c_str());

            MessageBoxA(m_window->getSystemHandle(), l_log.c_str(), NULL, MB_OK | MB_ICONEXCLAMATION);
            std::exit(-1);
        }

        GLState::Finish(); // Wait for something

        l_log.assign("OpenGL ");
        l_log.append(reinterpret_cast<const char*>(GLSetting::GetString(GL_VERSION)));
        l_log.append(", ");
        l_log.append(reinterpret_cast<const char*>(GLSetting::GetString(GL_RENDERER)));
        l_log.append(", ");
        l_log.append("GLEW ");
        l_log.append(reinterpret_cast<const char*>(glewGetString(GLEW_VERSION)));
        GetCore()->GetLogManager()->Log(l_log.c_str());

        // Set default icon
        sf::Image l_image;
        if(l_image.loadFromFile("roc_icon.png"))
        {
            sf::Vector2u l_size = l_image.getSize();
            m_window->setIcon(l_size.x, l_size.y, l_image.getPixelsPtr());
        }
    }

    Manager::Start();
}

void ROC::SfmlManager::Stop()
{
    if(IsActive())
    {
        GLSetting::MemoryCleanup();
        m_window->setActive(false);
        m_window->close();
        delete m_window;
    }

    Manager::Stop();
}

void ROC::SfmlManager::GetWindowPosition(glm::ivec2 &p_pos) const
{
    if(!IsActive()) return;

    sf::Vector2i l_position = m_window->getPosition();
    p_pos.x = l_position.x;
    p_pos.y = l_position.y;
}

void ROC::SfmlManager::SetWindowPosition(const glm::ivec2 &p_pos)
{
    if(!IsActive()) return;

    sf::Vector2i l_position(p_pos.x, p_pos.y);
    m_window->setPosition(l_position);
}

void ROC::SfmlManager::GetWindowSize(glm::ivec2 &p_size) const
{
    if(!IsActive()) return;

    sf::Vector2u l_size = m_window->getSize();
    p_size.x = static_cast<int>(l_size.x);
    p_size.y = static_cast<int>(l_size.y);
}

void ROC::SfmlManager::CloseWindow()
{
    m_active = false;
}

void ROC::SfmlManager::SetVSync(bool p_sync)
{
    if(!IsActive()) return;

    m_window->setVerticalSyncEnabled(p_sync);
}

void ROC::SfmlManager::SetFramelimit(unsigned int p_fps)
{
    if(!IsActive()) return;

    if(p_fps != m_frameLimit)
    {
        m_frameLimit = p_fps;
        m_window->setFramerateLimit(m_frameLimit);
        GetCore()->GetPhysicsManager()->UpdateWorldSteps(m_frameLimit);
    }
}

unsigned int ROC::SfmlManager::GetFramelimit() const
{
    return m_frameLimit;
}

void ROC::SfmlManager::SetTitle(const std::string &p_title)
{
    if(!IsActive()) return;
    m_title.assign(p_title);
    sf::String l_title = sf::String::fromUtf8(m_title.begin(), m_title.end());
    m_window->setTitle(l_title);
}

bool ROC::SfmlManager::SetIcon(const std::string &p_path)
{
    if(!IsActive()) return false;

    bool l_result = false;

    sf::Image l_image;
    if(l_image.loadFromFile(p_path))
    {
        sf::Vector2u l_size = l_image.getSize();
        m_window->setIcon(l_size.x, l_size.y, l_image.getPixelsPtr());
        l_result = true;
    }
    return l_result;
}

void ROC::SfmlManager::RequestFocus()
{
    if(!IsActive()) return;
    m_window->requestFocus();
}

bool ROC::SfmlManager::GetFocusState() const
{
    if(!IsActive()) return false;
    return m_window->hasFocus();
}

bool ROC::SfmlManager::GetInputEnabled() const
{
    return m_inputEnabled;
}

void ROC::SfmlManager::SetInputEnabled(bool p_state)
{
    if(!IsActive()) return;
    m_inputEnabled = p_state;
}

void ROC::SfmlManager::SetCursorMode(bool p_visible, bool p_lock)
{
    if(!IsActive()) return;

    m_window->setMouseCursorGrabbed(p_lock);
    m_window->setMouseCursorVisible(p_visible);
}

void ROC::SfmlManager::GetCursorPosition(glm::ivec2 &p_pos) const
{
    if(!IsActive()) return;

    sf::Vector2i l_position = sf::Mouse::getPosition(*m_window);
    std::memcpy(&p_pos, &l_position, sizeof(glm::ivec2));
}

void ROC::SfmlManager::SetCursorPosition(const glm::ivec2 &p_pos)
{
    if(!IsActive()) return;
    sf::Mouse::setPosition(reinterpret_cast<const sf::Vector2i&>(p_pos), *m_window);
}

void ROC::SfmlManager::GetClipboardString(std::string &p_str)
{
    if(!IsActive()) return;

    p_str.assign(sf::Clipboard::getString());
}

void ROC::SfmlManager::SetClipboardString(const std::string &p_str)
{
    if(!IsActive()) return;
    sf::Clipboard::setString(p_str);
}

bool ROC::SfmlManager::IsKeyPressed(int p_key)
{
    if(!IsActive()) return false;
    return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(p_key));
}

bool ROC::SfmlManager::IsMouseKeyPressed(int p_key)
{
    if(!IsActive()) return false;
    return sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(p_key));
}

bool ROC::SfmlManager::IsJoypadConnected(unsigned int p_jp)
{
    if(!IsActive()) return false;
    return sf::Joystick::isConnected(p_jp);
}

bool ROC::SfmlManager::GetJoypadButtonState(unsigned int p_jp, unsigned int p_button)
{
    return sf::Joystick::isButtonPressed(p_jp, p_button);
}

unsigned int ROC::SfmlManager::GetJoypadButtonCount(unsigned int p_jp)
{
    if(!IsActive()) return 0U;
    return sf::Joystick::getButtonCount(p_jp);
}

bool ROC::SfmlManager::CheckJoypadAxis(unsigned int p_jp, unsigned int p_axis)
{
    if(!IsActive()) return false;
    return sf::Joystick::hasAxis(p_jp, static_cast<sf::Joystick::Axis>(p_axis));
}

float ROC::SfmlManager::GetJoypadAxisValue(unsigned int p_jp, unsigned int p_axis)
{
    if(!IsActive()) return 0.f;
    return sf::Joystick::getAxisPosition(p_jp, static_cast<sf::Joystick::Axis>(p_axis));
}

float ROC::SfmlManager::GetTime() const
{
    return m_time;
}

void ROC::SfmlManager::UpdateWindow()
{
    if(!IsActive()) return;
    m_window->display();
}

bool ROC::SfmlManager::DoPulse()
{
    if(!IsActive()) return false;

    m_time = m_clock.getElapsedTime().asSeconds();

    bool l_ignoreCursorEvent = false;
    while(m_window->pollEvent(m_event))
    {
        switch(m_event.type)
        {
            case sf::Event::Closed:
                m_active = false;
                break;
            case sf::Event::Resized:
            {
                glm::ivec2 l_size(static_cast<int>(m_event.size.width), static_cast<int>(m_event.size.height));
                GetCore()->GetRenderManager()->UpdateScreenSize(l_size);

                m_arguments->Push(l_size.x);
                m_arguments->Push(l_size.y);
                GetCore()->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnWindowResize, m_arguments);
                m_arguments->Clear();
            } break;
            case sf::Event::GainedFocus: case sf::Event::LostFocus:
            {
                m_arguments->Push(m_event.type == sf::Event::GainedFocus);
                GetCore()->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnWindowFocus, m_arguments);
                m_arguments->Clear();
            } break;
            case sf::Event::KeyPressed: case sf::Event::KeyReleased:
            {
                if(m_event.key.code != -1)
                {
                    m_arguments->Push(g_keyNames[m_event.key.code]);
                    m_arguments->Push(m_event.type == sf::Event::KeyPressed);
                    GetCore()->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnKeyPress, m_arguments);
                    m_arguments->Clear();
                }
            } break;
            case sf::Event::TextEntered:
            {
                if(m_inputEnabled && m_event.text.unicode > 31 && !(m_event.text.unicode >= 127 && m_event.text.unicode <= 160))
                {
                    sf::String l_text(m_event.text.unicode);
                    std::basic_string<unsigned char> l_utf8 = l_text.toUtf8();
                    std::string l_input(l_utf8.begin(), l_utf8.end());

                    m_arguments->Push(l_input);
                    GetCore()->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnTextInput, m_arguments);
                    m_arguments->Clear();
                }
            } break;
            case sf::Event::MouseMoved:
            {
                if(!l_ignoreCursorEvent) // Prevent loop if cursor position is changed in onCursorMove event
                {
                    m_arguments->Push(m_event.mouseMove.x);
                    m_arguments->Push(m_event.mouseMove.y);
                    GetCore()->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnCursorMove, m_arguments);
                    m_arguments->Clear();
                    l_ignoreCursorEvent = true;
                }
            } break;
            case sf::Event::MouseEntered: case sf::Event::MouseLeft:
            {
                m_arguments->Push(m_event.type == sf::Event::MouseEntered);
                GetCore()->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnCursorEnter, m_arguments);
                m_arguments->Clear();
            } break;
            case sf::Event::MouseButtonPressed: case sf::Event::MouseButtonReleased:
            {
                m_arguments->Push(g_mouseKeyNames[m_event.mouseButton.button]);
                m_arguments->Push(m_event.type == sf::Event::MouseButtonPressed);
                GetCore()->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnMouseKeyPress, m_arguments);
                m_arguments->Clear();
            } break;
            case sf::Event::MouseWheelScrolled:
            {
                m_arguments->Push(m_event.mouseWheelScroll.wheel);
                m_arguments->Push(m_event.mouseWheelScroll.delta);
                GetCore()->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnMouseScroll, m_arguments);
                m_arguments->Clear();
            } break;
            case sf::Event::JoystickConnected: case sf::Event::JoystickDisconnected:
            {
                m_arguments->Push(static_cast<int>(m_event.joystickConnect.joystickId));
                m_arguments->Push(m_event.type == sf::Event::JoystickConnected);
                GetCore()->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnJoypadStateChange, m_arguments);
                m_arguments->Clear();
            } break;
            case sf::Event::JoystickButtonPressed: case sf::Event::JoystickButtonReleased:
            {
                m_arguments->Push(static_cast<int>(m_event.joystickButton.joystickId));
                m_arguments->Push(static_cast<int>(m_event.joystickButton.button));
                m_arguments->Push(m_event.type == sf::Event::JoystickButtonPressed);
                GetCore()->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnJoypadButton, m_arguments);
                m_arguments->Clear();
            } break;
            case sf::Event::JoystickMoved:
            {
                m_arguments->Push(static_cast<int>(m_event.joystickMove.joystickId));
                m_arguments->Push(g_joypadAxisNames[m_event.joystickMove.axis]);
                m_arguments->Push(m_event.joystickMove.position);
                GetCore()->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnJoypadAxis, m_arguments);
                m_arguments->Clear();
            } break;
        }
    }
    return m_active;
}

// ROC::ISfmlManager
const char* ROC::SfmlManager::GetTitle() const
{
    return m_title.c_str();
}
void ROC::SfmlManager::SetTitle(const char* p_title)
{
    std::string l_title(p_title);
    SetTitle(l_title);
}
bool ROC::SfmlManager::SetIcon(const char* p_path)
{
    std::string l_path(p_path);
    return SetIcon(l_path);
}
const char* ROC::SfmlManager::GetClipboardString()
{
    GetClipboardString(m_lastClipboard);
    return m_lastClipboard.c_str();
}
void ROC::SfmlManager::SetClipboardString(const char* p_str)
{
    std::string l_str(p_str);
    SetClipboardString(l_str);
}

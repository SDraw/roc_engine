#pragma once
#include "Managers/Manager.h"

namespace ROC
{

class ConfigManager final : public Manager
{
    bool m_logging;
    bool m_fullscreen;
    int m_antialiasing;
    glm::ivec2 m_windowSize;
    unsigned int m_fpsLimit;
    bool m_vsync;
    std::vector<std::string> m_modules;

    ConfigManager(const ConfigManager &that) = delete;
    ConfigManager& operator=(const ConfigManager &that) = delete;
public:
    ConfigManager(Core *p_core);
    ~ConfigManager() = default;

    // ROC::Manager
    void Start() override;

    bool IsLogEnabled() const;
    bool IsFullscreenEnabled() const;
    int GetAntialiasing() const;
    const glm::ivec2& GetWindowSize() const;
    unsigned int GetFPSLimit() const;
    bool GetVSync() const;
    const std::vector<std::string>& GetModules() const;
};

}

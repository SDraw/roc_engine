#pragma once
#include "Interfaces/ILogManager.h"
#include "Managers/Manager.h"

namespace ROC
{

class LogManager final : public ILogManager, public Manager
{
    std::ofstream m_log;
    bool m_enabled;

    LogManager(const LogManager &that) = delete;
    LogManager& operator=(const LogManager &that) = delete;
public:
    explicit LogManager(Core *p_core);
    ~LogManager() = default;

    // ROC::Manager
    void Start() override;
    void Stop() override;

    void Log(const char *p_text);
};

}

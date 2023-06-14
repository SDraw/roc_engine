#include "stdafx.h"

#include "Managers/LogManager.h"
#include "Core/Core.h"

#include "Managers/ConfigManager.h"

ROC::LogManager::LogManager(Core *p_core) : Manager(p_core)
{
    m_enabled = false;
}

void ROC::LogManager::Start()
{
    if(!IsActive())
    {
        m_enabled = GetCore()->GetConfigManager()->IsLogEnabled();
        if(m_enabled)
        {
            m_log.open("log.txt", std::ofstream::out);
            if(m_log.fail())
            {
                m_log.clear();
                m_enabled = false;
            }
        }
    }

    Manager::Start();
}

void ROC::LogManager::Stop()
{
    if(IsActive() && m_enabled)
    {
        std::string l_log("Application closed");
        Log(l_log.c_str());
        m_log.flush();
        m_log.close();

        m_enabled = false;
    }

    Manager::Stop();
}

void ROC::LogManager::Log(const char* p_text)
{
    if(!IsActive()) return;

    std::time_t l_timeRaw = std::time(nullptr);
    tm l_time = { 0 };
    localtime_s(&l_time, &l_timeRaw);
#ifdef _DEBUG
    std::cout << "[" << l_time.tm_hour << ":" << l_time.tm_min << ":" << l_time.tm_sec << "] " << p_text << std::endl;
#endif
    if(m_enabled)
        m_log << "[" << l_time.tm_hour << ":" << l_time.tm_min << ":" << l_time.tm_sec << "] " << p_text << std::endl;
}

#include "stdafx.h"

#include "Managers/ConfigManager.h"

#include "Utils/EnumUtils.h"

namespace ROC
{

const std::vector<std::string> g_configAttributes
{
    "antialiasing", "dimension", "fullscreen", "logging", "fpslimit", "vsync", "module"
};
enum ConfigAttribute : size_t
{
    CA_Antialiasing = 0U,
    CA_Dimension,
    CA_Fullscreen,
    CA_Logging,
    CA_FPSLimit,
    CA_VSync,
    CA_Module
};

}

ROC::ConfigManager::ConfigManager(Core *p_core) : Manager(p_core)
{
    m_logging = false;
    m_fullscreen = false;
    m_antialiasing = 0;
    m_windowSize = glm::ivec2(854, 480);
    m_fpsLimit = 60U;
    m_vsync = false;
}

void ROC::ConfigManager::Start()
{
    if(!IsActive())
    {
        pugi::xml_document *l_settings = new pugi::xml_document();
        if(l_settings->load_file("settings.xml"))
        {
            pugi::xml_node l_root = l_settings->child("settings");
            if(l_root)
            {
                for(pugi::xml_node l_node = l_root.child("setting"); l_node; l_node = l_node.next_sibling("setting"))
                {
                    pugi::xml_attribute l_attribName = l_node.attribute("name");
                    pugi::xml_attribute l_attribValue = l_node.attribute("value");
                    if(l_attribName && l_attribValue)
                    {
                        switch(EnumUtils::ReadEnumVector(l_attribName.as_string(), g_configAttributes))
                        {
                            case ConfigAttribute::CA_Antialiasing:
                                m_antialiasing = glm::clamp(l_attribValue.as_int(0), 0, std::numeric_limits<int>::max());
                                break;
                            case ConfigAttribute::CA_Dimension:
                            {
                                std::string l_size(l_attribValue.as_string("854x480"));
                                std::replace(l_size.begin(), l_size.end(), 'x', ' ');
                                std::stringstream l_sizeStream(l_size);
                                l_sizeStream >> m_windowSize.x >> m_windowSize.y;
                                if(l_sizeStream.fail())
                                {
                                    m_windowSize.x = 854;
                                    m_windowSize.y = 480;
                                }
                            } break;
                            case ConfigAttribute::CA_Fullscreen:
                                m_fullscreen = l_attribValue.as_bool(false);
                                break;
                            case ConfigAttribute::CA_Logging:
                                m_logging = l_attribValue.as_bool(false);
                                break;
                            case ConfigAttribute::CA_FPSLimit:
                            {
                                m_fpsLimit = l_attribValue.as_uint(60U);
                                if(m_fpsLimit == 0U) m_fpsLimit = 60U;
                            } break;
                            case ConfigAttribute::CA_VSync:
                                m_vsync = l_attribValue.as_bool(false);
                                break;
                            case ConfigAttribute::CA_Module:
                            {
                                std::string l_module(l_attribValue.as_string());
                                if(!l_module.empty()) m_modules.push_back(l_module);
                            } break;
                        }
                    }
                }
            }
        }
        delete l_settings;
    }

    Manager::Start();
}

bool ROC::ConfigManager::IsLogEnabled() const
{
    return m_logging;
}

bool ROC::ConfigManager::IsFullscreenEnabled() const
{
    return m_fullscreen;
}

int ROC::ConfigManager::GetAntialiasing() const
{
    return m_antialiasing;
}

const glm::ivec2& ROC::ConfigManager::GetWindowSize() const
{
    return m_windowSize;
}

unsigned int ROC::ConfigManager::GetFPSLimit() const
{
    return m_fpsLimit;
}

bool ROC::ConfigManager::GetVSync() const
{
    return m_vsync;
}

const std::vector<std::string>& ROC::ConfigManager::GetModules() const
{
    return m_modules;
}

#include "stdafx.h"
#include "Core.h"
#include "LuaHandler.h"
#include "Logger.h"

Core* Core::ms_instance = nullptr;

Core::Core()
{
    m_luaHandler = new LuaHandler();
    m_engineCore = nullptr;
}
Core::~Core()
{
    delete m_luaHandler;
}

void Core::Init(ROC::ICore *p_core)
{
    if(!ms_instance)
    {
        LuaHandler::InitDefs();

        ms_instance = new Core();
        ms_instance->m_engineCore = p_core;

        // Load scripts
        {
            pugi::xml_document *l_settings = new pugi::xml_document();
            if(l_settings->load_file("module_lua.xml"))
            {
                pugi::xml_node l_root = l_settings->child("module");
                if(l_root)
                {
                    for(pugi::xml_node l_node = l_root.child("script"); l_node; l_node = l_node.next_sibling("script"))
                    {
                        pugi::xml_attribute l_attrib = l_node.attribute("src");
                        if(l_attrib)
                        {
                            std::string l_path(l_attrib.as_string());
                            ms_instance->m_luaHandler->LoadScript(l_path);

                            std::string l_info("Script '");
                            l_info.append(l_path);
                            l_info.append("' is loaded");
                            Logger::Log(l_info);
                        }
                    }
                }
            }
            delete l_settings;
        }

        // Parse events
        ms_instance->m_luaHandler->ParseEvents();
    }
}

Core* Core::GetInstance()
{
    return ms_instance;
}

ROC::ICore* Core::GetEngineCore()
{
    return m_engineCore;
}

void Core::RecieveEvent(ModuleEvent p_event, const ROC::ICustomArguments *p_args)
{
    m_luaHandler->CallEvent(p_event, p_args);
}

void Core::DoPulse()
{
    m_luaHandler->PerformGC();
}

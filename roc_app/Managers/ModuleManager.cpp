#include "stdafx.h"

#include "Managers/ModuleManager.h"
#include "Core/Core.h"
#include "Interfaces/IModule.h"
#include "Utils/CustomArguments.h"

#include "Managers/ConfigManager.h"
#include "Managers/LogManager.h"

ROC::ModuleManager::ModuleManager(Core *p_core) : Manager(p_core)
{
}

void ROC::ModuleManager::Start()
{
    if(!IsActive())
    {
        std::string l_log;

        auto &l_modules = GetCore()->GetConfigManager()->GetModules();
        for(auto &l_modulePath : l_modules)
        {
            HMODULE l_dll = LoadLibraryA(l_modulePath.c_str());
            if(l_dll)
            {
                ModuleInitFunc l_func = reinterpret_cast<ModuleInitFunc>(GetProcAddress(l_dll, "ModuleInit"));
                if(l_func)
                {
                    IModule *l_module = (*l_func)(Manager::GetCore());
                    if(l_module)
                    {
                        m_modules.push_back(l_module);
                        m_libraries.push_back(l_dll);

                        l_log.assign("Module '");
                        l_log.append(l_modulePath);
                        l_log.append("' is loaded");
                    }
                    else
                    {
                        FreeLibrary(l_dll);

                        l_log.assign("No module interface in module '");
                        l_log.append(l_modulePath);
                        l_log.push_back('\'');
                    }
                }
                else
                {
                    FreeLibrary(l_dll);

                    l_log.assign("Unable to find entry point in module '");
                    l_log.append(l_modulePath);
                    l_log.push_back('\'');
                }
            }
            else
            {
                l_log.assign("Unable to load '");
                l_log.append(l_modulePath);
                l_log.append("' module");
            }
            GetCore()->GetLogManager()->Log(l_log.c_str());
        }
    }

    Manager::Start();
}

void ROC::ModuleManager::Stop()
{
    if(IsActive())
    {
        for(auto l_library : m_libraries) FreeLibrary(l_library);
        m_libraries.clear();
        m_modules.clear();
    }

    Manager::Stop();
}

void ROC::ModuleManager::SignalGlobalEvent(unsigned char p_event, const CustomArguments *p_args) const
{
    if(!IsActive()) return;
    for(auto l_module : m_modules) l_module->RecieveEvent(static_cast<ROC::IModule::ModuleEvent>(p_event), p_args);
}

void ROC::ModuleManager::DoPulse() const
{
    if(!IsActive()) return;
    for(auto l_module : m_modules) l_module->DoPulse();
}

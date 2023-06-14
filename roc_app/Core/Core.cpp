#include "stdafx.h"

#include "Core/Core.h"
#include "Managers/ConfigManager.h"
#include "Managers/ElementManager.h"
#include "Managers/LogManager.h"
#include "Managers/ModuleManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/PreRenderManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Managers/SfmlManager.h"
#include "Managers/SoundManager.h"
#include "Utils/CustomArguments.h"

#include "Interfaces/IModule.h"
#include "Utils/SystemTick.h"

ROC::Core *ROC::Core::ms_instance = nullptr;

ROC::Core::Core()
{
    m_configManager = new ConfigManager(this);
    m_logManager = new LogManager(this);
    m_elementManager = new ElementManager(this);
    m_soundManager = new SoundManager(this);
    m_physicsManager = new PhysicsManager(this);
    m_sfmlManager = new SfmlManager(this);
    m_preRenderManager = new PreRenderManager(this);
    m_renderManager = new RenderManager(this);
    m_moduleManager = new ModuleManager(this);

    m_state = false;
    m_arguments = new CustomArguments();
}

ROC::Core::~Core()
{
    delete m_moduleManager;
    delete m_soundManager;
    delete m_physicsManager;
    delete m_elementManager;
    delete m_renderManager;
    delete m_preRenderManager;
    delete m_sfmlManager;
    delete m_logManager;
    delete m_configManager;
    delete m_arguments;
}

bool ROC::Core::Init()
{
    if(!ms_instance)
    {
        ms_instance = new Core();
        SystemTick::Init();

        // Start managers
        ms_instance->GetConfigManager()->Start();
        ms_instance->GetLogManager()->Start();
        ms_instance->GetElementManager()->Start();
        ms_instance->GetSoundManager()->Start();
        ms_instance->GetPhysicsManager()->Start();
        ms_instance->GetSfmlManager()->Start();
        ms_instance->GetPreRenderManager()->Start();
        ms_instance->GetRenderManager()->Start();
        ms_instance->GetModuleManager()->Start();

        ms_instance->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnEngineStart, ms_instance->m_arguments);

        ms_instance->m_state = true;
    }
    return (ms_instance != nullptr);
}

void ROC::Core::Terminate()
{
    if(ms_instance)
    {
        ms_instance->m_state = false;

        // Unload modules
        ms_instance->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnEngineStop, ms_instance->m_arguments);

        // Terminate managers
        ms_instance->GetModuleManager()->Stop();
        ms_instance->GetSoundManager()->Stop();
        ms_instance->GetPhysicsManager()->Stop();
        ms_instance->GetElementManager()->Stop();
        ms_instance->GetRenderManager()->Stop();
        ms_instance->GetPreRenderManager()->Stop();
        ms_instance->GetSfmlManager()->Stop();
        ms_instance->GetLogManager()->Stop();
        ms_instance->GetConfigManager()->Stop();

        delete ms_instance;
        ms_instance = nullptr;
    }
}

ROC::Core* ROC::Core::GetInstance()
{
    return ms_instance;
}

ROC::ConfigManager* ROC::Core::GetConfigManager() const
{
    return m_configManager;
}

ROC::ElementManager* ROC::Core::GetElementManager() const
{
    return m_elementManager;
}

ROC::LogManager* ROC::Core::GetLogManager() const
{
    return m_logManager;
}

ROC::ModuleManager* ROC::Core::GetModuleManager() const
{
    return m_moduleManager;
}

ROC::PhysicsManager* ROC::Core::GetPhysicsManager() const
{
    return m_physicsManager;
}

ROC::RenderManager* ROC::Core::GetRenderManager() const
{
    return m_renderManager;
}

ROC::PreRenderManager* ROC::Core::GetPreRenderManager() const
{
    return m_preRenderManager;
}

ROC::SfmlManager* ROC::Core::GetSfmlManager() const
{
    return m_sfmlManager;
}

ROC::SoundManager* ROC::Core::GetSoundManager() const
{
    return m_soundManager;
}

bool ROC::Core::DoPulse()
{
    if(!m_state) return false;

    SystemTick::UpdateTick();
    m_moduleManager->DoPulse();
    m_state = (m_state && m_sfmlManager->DoPulse());
    m_preRenderManager->DoPulse_S1();
    m_physicsManager->DoPulse();
    m_preRenderManager->DoPulse_S2();
    m_renderManager->DoPulse(); // Render
    return m_state;
}

// ROC::ICore
ROC::IElementManager* ROC::Core::GetIElementManager() const
{
    return m_elementManager;
}

ROC::ILogManager* ROC::Core::GetILogManager() const
{
    return m_logManager;
}

ROC::IPhysicsManager* ROC::Core::GetIPhysicsManager() const
{
    return m_physicsManager;
}

ROC::IRenderManager* ROC::Core::GetIRenderManager() const
{
    return m_renderManager;
}

ROC::ISfmlManager* ROC::Core::GetISfmlManager() const
{
    return m_sfmlManager;
}

ROC::ISoundManager* ROC::Core::GetISoundManager() const
{
    return m_soundManager;
}

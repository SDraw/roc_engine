#include "stdafx.h"
#include "Managers/Manager.h"

ROC::Manager::Manager(Core *p_core)
{
    m_core = p_core;
    m_started = false;
}

void ROC::Manager::Start()
{
    if(!m_started)
        m_started = true;
}

void ROC::Manager::Stop()
{
    if(m_started)
        m_started = false;
}

bool ROC::Manager::IsActive() const
{
    return m_started;
}

ROC::Core* ROC::Manager::GetCore()
{
    return m_core;
}

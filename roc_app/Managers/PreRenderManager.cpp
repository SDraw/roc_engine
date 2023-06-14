#include "stdafx.h"

#include "Managers/PreRenderManager.h"
#include "Core/Core.h"
#include "Elements/Model/Model.h"
#include "Utils/CustomArguments.h"

#include "Managers/ModuleManager.h"
#include "Interfaces/IModule.h"

ROC::PreRenderManager::PreRenderManager(Core *p_core) : Manager(p_core)
{
    m_arguments = new CustomArguments();
}

ROC::PreRenderManager::~PreRenderManager()
{
    delete m_arguments;
}

void ROC::PreRenderManager::Stop()
{
    if(IsActive())
    {
        m_models.clear();
    }

    Manager::Stop();
}

void ROC::PreRenderManager::AddModel(Model *p_model)
{
    if(!IsActive()) return;

    m_models.push_back(p_model);
}

void ROC::PreRenderManager::RemoveModel(Model *p_model)
{
    if(!IsActive()) return;

    auto l_searchIter = std::find(m_models.begin(), m_models.end(), p_model);
    if(l_searchIter != m_models.end()) m_models.erase(l_searchIter);
}

void ROC::PreRenderManager::DoPulse_S1()
{
    if(!IsActive()) return;

    GetCore()->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnPreRender, m_arguments);

    for(auto l_model : m_models)
    {
        l_model->Update(Model::MUS_Matrix);
        l_model->Update(Model::MUS_SkeletonStatic);
    }
}

void ROC::PreRenderManager::DoPulse_S2()
{
    if(!IsActive()) return;

    for(auto l_model : m_models)
    {
        l_model->Update(Model::MUS_Matrix);
        l_model->Update(Model::MUS_SkeletonDynamic);
    }
}

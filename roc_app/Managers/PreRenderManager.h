#pragma once
#include "Managers/Manager.h"

namespace ROC
{

class Model;
class CustomArguments;

class PreRenderManager final : public Manager
{
    std::vector<Model*> m_models;

    CustomArguments *m_arguments;

    PreRenderManager(const PreRenderManager &that) = delete;
    PreRenderManager& operator=(const PreRenderManager &that) = delete;
public:
    explicit PreRenderManager(Core *p_core);
    ~PreRenderManager();

    // ROC::Manager
    void Stop() override;

    void AddModel(Model *p_model);
    void RemoveModel(Model *p_model);

    void DoPulse_S1();
    void DoPulse_S2();
};

}

#include "stdafx.h"

#include "Elements/Scene/Scene.h"
#include "Elements/Camera.h"
#include "Elements/Light.h"
#include "Elements/Model/Model.h"
#include "Elements/RenderTarget.h"
#include "Elements/Shader/Shader.h"

#include "Elements/Model/Model.h"

ROC::Scene::Scene()
{
    m_elementType = ET_Scene;

    m_camera = nullptr;
    m_renderTarget = nullptr;
    m_shader = nullptr;

    m_active = false;
}

bool ROC::Scene::SetCamera(Camera *p_cam)
{
    bool l_result = false;

    if(m_camera != p_cam)
    {
        if(m_camera) Element::Unlink(this, m_camera);

        m_camera = p_cam;
        Element::Link(this, m_camera);

        if(m_active) m_camera->Update();

        l_result = true;
    }
    return l_result;
}

bool ROC::Scene::RemoveCamera()
{
    bool l_result = false;
    if(m_camera)
    {
        Element::Unlink(this, m_camera);
        m_camera = nullptr;
        l_result = true;
    }
    return l_result;
}

ROC::Camera* ROC::Scene::GetCamera() const
{
    return m_camera;
}

bool ROC::Scene::SetRenderTarget(RenderTarget *p_rt)
{
    bool l_result = false;
    if(m_renderTarget != p_rt)
    {
        if(m_renderTarget)
        {
            if(m_active) RenderTarget::Disable();
            Element::Unlink(this, m_renderTarget);
        }

        m_renderTarget = p_rt;
        Element::Link(this, m_renderTarget);

        if(m_active) m_renderTarget->Enable();

        l_result = true;
    }
    return l_result;
}

bool ROC::Scene::RemoveRenderTarget()
{
    bool l_result = false;
    if(m_renderTarget)
    {
        if(m_active) RenderTarget::Disable();

        Element::Unlink(this, m_renderTarget);
        m_renderTarget = nullptr;

        l_result = true;
    }
    return l_result;
}

ROC::RenderTarget* ROC::Scene::GetRenderTarget() const
{
    return m_renderTarget;
}

bool ROC::Scene::SetShader(Shader *p_shader)
{
    if(m_shader) RemoveShader();

    m_shader = p_shader;
    Element::Link(this, m_shader);
    return (m_shader != nullptr);
}

bool ROC::Scene::RemoveShader()
{
    if(m_shader)
    {
        Element::Unlink(this, m_shader);
        m_shader = nullptr;
    }
    return (!m_shader);
}

ROC::Shader* ROC::Scene::GetShader() const
{
    return m_shader;
}

bool ROC::Scene::AddLight(Light *p_light)
{
    bool l_result = false;
    if(!HasLight(p_light))
    {
        m_lights.push_back(p_light);
        Element::Link(this, p_light);

        l_result = true;
    }
    return l_result;
}

bool ROC::Scene::HasLight(Light *p_light) const
{
    bool l_result = (std::find(m_lights.begin(), m_lights.end(), p_light) != m_lights.end());
    return l_result;
}

bool ROC::Scene::RemoveLight(Light *p_light)
{
    bool l_result = false;
    auto l_searchIter = std::find(m_lights.begin(), m_lights.end(), p_light);
    if(l_searchIter != m_lights.end())
    {
        m_lights.erase(l_searchIter);
        Element::Unlink(this, p_light);
        l_result = true;
    }
    return l_result;
}

size_t ROC::Scene::GetLightsCount() const
{
    return m_lights.size();
}

const std::vector<ROC::Light*>& ROC::Scene::GetLights() const
{
    return m_lights;
}

bool ROC::Scene::AddModel(Model *p_model)
{
    bool l_result = false;
    auto l_searchIter = std::find(m_models.begin(), m_models.end(), p_model);
    if(l_searchIter == m_models.end())
    {
        m_models.push_back(p_model);
        Element::Link(this, p_model);
        l_result = true;
    }
    return l_result;
}

bool ROC::Scene::RemoveModel(Model *p_model)
{
    bool l_result = false;
    auto l_searchIter = std::find(m_models.begin(), m_models.end(), p_model);
    if(l_searchIter != m_models.end())
    {
        m_models.erase(l_searchIter);
        Element::Unlink(this, p_model);
        l_result = true;
    }
    return l_result;
}

bool ROC::Scene::HasModel(Model *p_model) const
{
    auto l_searchIter = std::find(m_models.begin(), m_models.end(), p_model);
    return (l_searchIter != m_models.end());
}

const std::vector<ROC::Model*>& ROC::Scene::GetModels() const
{
    return m_models;
}

bool ROC::Scene::IsRenderValid() const
{
    return ((m_camera != nullptr) && (m_shader != nullptr));
}

void ROC::Scene::Enable()
{
    if(!m_active)
    {
        for(auto l_light : m_lights) l_light->UpdateMatrix();

        if(m_camera)
        {
            m_camera->Update();

            // Set visibilty for models
            glm::vec3 l_pos;
            btTransform l_transform = btTransform::getIdentity();
            for(auto l_model : m_models)
            {
                l_transform.setFromOpenGLMatrix(glm::value_ptr(l_model->GetMatrix()));
                const btVector3 &l_btPos = l_transform.getOrigin();
                l_pos.x = l_btPos.x();
                l_pos.y = l_btPos.y();
                l_pos.z = l_btPos.z();

                bool l_visible = m_camera->IsInFrustum(l_pos, l_model->GetBoundSphereRadius());
                l_model->SetVisibility(l_visible);

                float l_dist = glm::distance(m_camera->GetPosition(), l_pos);
                l_model->SetVisibilityDistance(l_dist);
            }

            // Sort by mesh and distance to camera
            std::sort(m_models.begin(), m_models.end(), SceneModelSorting);
        }
        if(m_renderTarget) m_renderTarget->Enable();
        else RenderTarget::Disable();

        m_active = true;
    }
}

void ROC::Scene::Disable()
{
    if(m_active)
    {
        if(m_renderTarget) RenderTarget::Disable();
        m_active = false;
    }
}

bool ROC::Scene::IsActive() const
{
    return m_active;
}

bool ROC::Scene::SceneModelSorting(const Model *p_modelA, const Model *p_modelB)
{
    if(p_modelA->GetMesh() != p_modelB->GetMesh())
    {
        return (p_modelA->GetMesh() < p_modelB->GetMesh());
    }
    return (p_modelA->GetVisibilityDistance() < p_modelB->GetVisibilityDistance());
}

void ROC::Scene::OnChildRemoved(Element *p_child)
{
    switch(p_child->GetElementType())
    {
        case ET_Camera:
        {
            m_camera = nullptr;
        } break;
        case ET_RenderTarget:
        {
            m_renderTarget = nullptr;
            if(m_active) RenderTarget::Fallback();
        } break;
        case ET_Light:
        {
            auto l_searchIter = std::find(m_lights.begin(), m_lights.end(), p_child);
            if(l_searchIter != m_lights.end()) m_lights.erase(l_searchIter);
        } break;
        case ET_Model:
        {
            auto l_searchIter = std::find(m_models.begin(), m_models.end(), p_child);
            if(l_searchIter != m_models.end()) m_models.erase(l_searchIter);
        } break;
        case ET_Shader:
        {
            m_shader = nullptr;
        } break;
    }

    Element::OnChildRemoved(p_child);
}

// ROC::IScene
bool ROC::Scene::SetICamera(ICamera *p_cam)
{
    return SetCamera(dynamic_cast<Camera*>(p_cam));
}

ROC::ICamera* ROC::Scene::GetICamera() const
{
    return m_camera;
}

bool ROC::Scene::SetIRenderTarget(IRenderTarget *p_rt)
{
    return SetRenderTarget(dynamic_cast<RenderTarget*>(p_rt));
}

ROC::IRenderTarget* ROC::Scene::GetIRenderTarget() const
{
    return m_renderTarget;
}

bool ROC::Scene::SetIShader(IShader *p_shader)
{
    return SetShader(dynamic_cast<Shader*>(p_shader));
}

bool ROC::Scene::RemoveIShader()
{
    return RemoveShader();
}

ROC::IShader* ROC::Scene::GetIShader() const
{
    return GetShader();
}
bool ROC::Scene::AddILight(ILight *p_light)
{
    return AddLight(dynamic_cast<Light*>(p_light));
}

bool ROC::Scene::RemoveILight(ILight *p_light)
{
    return RemoveLight(dynamic_cast<Light*>(p_light));
}

bool ROC::Scene::HasILight(ILight *p_light) const
{
    return HasLight(dynamic_cast<Light*>(p_light));
}

bool ROC::Scene::AddIModel(IModel *p_model)
{
    return AddModel(dynamic_cast<Model*>(p_model));
}

bool ROC::Scene::RemoveIModel(IModel *p_model)
{
    return RemoveModel(dynamic_cast<Model*>(p_model));
}

bool ROC::Scene::HasIModel(IModel *p_model) const
{
    return HasModel(dynamic_cast<Model*>(p_model));
}

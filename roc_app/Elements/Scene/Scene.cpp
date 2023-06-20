#include "stdafx.h"

#include "Elements/Scene/Scene.h"
#include "Elements/Camera.h"
#include "Elements/Light.h"
#include "Elements/Model/Model.h"
#include "Elements/Model/Skeleton.h"
#include "Elements/Mesh/Mesh.h"
#include "Elements/Mesh/Material.h"
#include "Elements/RenderTarget.h"
#include "Elements/Shader/Shader.h"

#include "Elements/Model/Model.h"
#include "GL/GLViewport.h"
#include "Utils/MathUtils.h"

namespace ROC
{

extern const glm::vec3 g_defaultDown;

}

ROC::Scene::Scene()
{
    m_elementType = ET_Scene;

    m_camera = nullptr;
    m_renderTarget = nullptr;
    m_shader = nullptr;
    m_shadowTarget = nullptr;
    m_shadowsQuality = 10;
    m_shadowCamera = new Camera(ROC::Camera::CPT_Orthogonal);
    m_shadowCamera->SetOrthoParams(glm::vec4(-32.f, 32.f, -32.f, 32.f));
    m_shadowCamera->SetDepth(glm::vec2(-50.f, 50.f));
}
ROC::Scene::~Scene()
{
    delete m_shadowCamera;
}

bool ROC::Scene::SetCamera(Camera *p_cam)
{
    bool l_result = false;

    if(m_camera != p_cam)
    {
        if(m_camera) Element::Unlink(this, m_camera);

        m_camera = p_cam;
        Element::Link(this, m_camera);

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
            Element::Unlink(this, m_renderTarget);

        m_renderTarget = p_rt;
        Element::Link(this, m_renderTarget);

        l_result = true;
    }
    return l_result;
}

bool ROC::Scene::RemoveRenderTarget()
{
    bool l_result = false;
    if(m_renderTarget)
    {
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

bool ROC::Scene::HasShadows() const
{
    return (m_shadowTarget != nullptr);
}

void ROC::Scene::SetShadows(bool p_state)
{
    if(p_state && !m_shadowTarget)
    {
        glm::ivec2 l_size(MathUtils::Power(2, m_shadowsQuality));
        m_shadowTarget = new RenderTarget();
        if(!m_shadowTarget->Create(ROC::RenderTarget::RTT_Shadow, l_size, ROC::Drawable::DFT_Linear))
        {
            delete m_shadowTarget;
            m_shadowTarget = nullptr;
        }
    }
    else if(!p_state && m_shadowTarget)
    {
        delete m_shadowTarget;
        m_shadowTarget = nullptr;
    }
}

void ROC::Scene::GetShadowsArea(glm::vec2 &p_area) const
{
    const glm::vec4 &l_ortho = m_shadowCamera->GetOrthoParams();
    p_area.x = abs(l_ortho.x) * 2.f;
    p_area.y = abs(l_ortho.z) * 2.f;
}
void ROC::Scene::SetShadowsArea(const glm::vec2 &p_area)
{
    glm::vec4 l_ortho(p_area.x * 0.5f, -p_area.x*0.5f, p_area.y * 0.5f, -p_area.y*0.5f);
    m_shadowCamera->SetOrthoParams(l_ortho);
}

void ROC::Scene::GetShadowsDepth(glm::vec2 &p_depth) const
{
    p_depth = m_shadowCamera->GetDepth();
}
void ROC::Scene::SetShadowsDepth(const glm::vec2 &p_depth)
{
    m_shadowCamera->SetDepth(p_depth);
}

int ROC::Scene::GetShadowsQuality() const
{
    return m_shadowsQuality;
}
void ROC::Scene::SetShadowsQuality(int p_quality)
{
    m_shadowsQuality = glm::clamp(p_quality, 1, 14);
    if(m_shadowTarget)
    {
        SetShadows(false);
        SetShadows(true);
    }
}

bool ROC::Scene::Draw(float p_time, const glm::ivec4 &p_viewport)
{
    bool l_result = false;

    // Update lights
    for(auto l_light : m_lights) l_light->UpdateMatrix();

    // Update cameras
    if(m_camera)
    {
        m_camera->Update();
        m_shadowCamera->SetPosition(m_camera->GetPosition());

        bool l_foundLight = false;
        for(auto l_light : m_lights)
        {
            if(l_light->GetType() == ROC::Light::LightType::LT_Directional)
            {
                m_shadowCamera->SetDirection(l_light->GetDirection());
                l_foundLight = true;
                break;
            }
        }
        if(!l_foundLight)
            m_shadowCamera->SetDirection(g_defaultDown);

        m_shadowCamera->Update();
    }

    if(m_camera && m_shader)
    {
        if(m_shadowTarget)
        {
            m_shadowTarget->BindBuffer(true);
            Shader::GetShadowShader()->Enable();
            Shader::GetShadowShader()->SetViewProjectionMatrix(m_shadowCamera->GetViewProjectionMatrix());
            SortModels(m_shadowCamera);
            DrawModels(Shader::GetShadowShader(), false);
            Shader::GetShadowShader()->Disable();
            RenderTarget::ResetBuffer();
        }

        if(m_renderTarget)
            m_renderTarget->BindBuffer();
        else
            GLViewport::SetViewport(p_viewport.x, p_viewport.z, p_viewport.y, p_viewport.w);

        m_shader->Enable();
        m_shader->SetTime(p_time);
        m_shader->SetProjectionMatrix(m_camera->GetProjectionMatrix());
        m_shader->SetViewMatrix(m_camera->GetViewMatrix());
        m_shader->SetViewProjectionMatrix(m_camera->GetViewProjectionMatrix());
        m_shader->SetCameraPosition(m_camera->GetPosition());
        m_shader->SetCameraDirection(m_camera->GetDirection());
        m_shader->SetShadowViewProjectionMatrix(m_shadowCamera->GetViewProjectionMatrix());
        m_shader->SetLightsData(m_lights);

        if(m_shadowTarget)
            m_shadowTarget->Bind(ROC::Shader::STS_Shadow);

        SortModels(m_camera);
        DrawModels(m_shader, true);

        m_shader->Disable();
        if(m_renderTarget)
            RenderTarget::ResetBuffer();

        l_result = true;
    }
    return l_result;
}

void ROC::Scene::DrawModels(Shader *p_shader, bool p_textured)
{
    for(const auto l_model : m_models)
    {
        if(l_model->IsVisible() && l_model->GetMesh())
        {
            p_shader->SetModelMatrix(l_model->GetMatrix());

            if(l_model->GetSkeleton())
            {
                p_shader->SetBoneMatrices(l_model->GetSkeleton()->GetPoseMatrices());
                p_shader->SetAnimated(true);
            }
            else p_shader->SetAnimated(false);

            for(const auto l_material : l_model->GetMesh()->GetMaterials())
            {
                if(!l_material->HasDepth() && p_textured) continue;

                p_shader->SetMaterialType(static_cast<int>(l_material->GetType()));
                p_shader->SetMaterialParam(l_material->GetParams());
                l_material->Draw(p_textured);
            }
        }
    }
}

void ROC::Scene::SortModels(Camera *p_camera)
{
    glm::vec3 l_pos;
    btTransform l_transform = btTransform::getIdentity();
    for(auto l_model : m_models)
    {
        l_transform.setFromOpenGLMatrix(glm::value_ptr(l_model->GetMatrix()));
        const btVector3 &l_btPos = l_transform.getOrigin();
        l_pos.x = l_btPos.x();
        l_pos.y = l_btPos.y();
        l_pos.z = l_btPos.z();

        bool l_visible = p_camera->IsInFrustum(l_pos, l_model->GetBoundSphereRadius());
        l_model->SetVisibility(l_visible);

        float l_dist = glm::distance(p_camera->GetPosition(), l_pos);
        l_model->SetVisibilityDistance(l_dist);
    }
    std::sort(m_models.begin(), m_models.end(), SceneModelSorting);
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

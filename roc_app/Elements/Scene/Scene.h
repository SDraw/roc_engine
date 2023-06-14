#pragma once
#include "Interfaces/IScene.h"
#include "Elements/Element.h"

namespace ROC
{

class Camera;
class Light;
class Model;
class RenderTarget;
class Shader;

class Scene final : public Element, public IScene
{
    Camera *m_camera;
    RenderTarget *m_renderTarget;
    Shader *m_shader;
    std::vector<Light*> m_lights;
    std::vector<Model*> m_models;

    bool m_active;

    Scene(const Scene &that) = delete;
    Scene& operator=(const Scene &that) = delete;

    static bool SceneModelSorting(const Model *p_modelA, const Model *p_modelB);

    // ROC::Element
    void OnChildRemoved(Element *p_child) override;

    // ROC::IScene
    bool SetICamera(ICamera *p_cam);
    ICamera* GetICamera() const;
    bool SetIRenderTarget(IRenderTarget *p_rt);
    IRenderTarget* GetIRenderTarget() const;
    bool SetIShader(IShader *p_shader);
    bool RemoveIShader();
    IShader* GetIShader() const;
    bool AddILight(ILight *p_light);
    bool RemoveILight(ILight *p_light);
    bool HasILight(ILight *p_light) const;
    bool AddIModel(IModel *p_model);
    bool RemoveIModel(IModel *p_model);
    bool HasIModel(IModel *p_model) const;
public:
    Scene();
    ~Scene() = default;

    bool SetCamera(Camera *p_cam);
    bool RemoveCamera();
    Camera* GetCamera() const;

    bool SetRenderTarget(RenderTarget *p_rt);
    bool RemoveRenderTarget();
    RenderTarget* GetRenderTarget() const;

    bool AddLight(Light *p_light);
    bool RemoveLight(Light *p_light);
    bool HasLight(Light *p_light) const;
    size_t GetLightsCount() const;

    bool SetShader(Shader *p_shader);
    bool RemoveShader();
    Shader* GetShader() const;

    bool AddModel(Model *p_model);
    bool RemoveModel(Model *p_model);
    bool HasModel(Model *p_model) const;

    bool IsActive() const;
    bool IsRenderValid() const;

    const std::vector<Light*>& GetLights() const;
    const std::vector<Model*>& GetModels() const;

    void Enable();
    void Disable();
};

}

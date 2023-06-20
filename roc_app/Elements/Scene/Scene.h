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

    int m_shadowsQuality;
    RenderTarget *m_shadowTarget;
    Camera *m_shadowCamera;

    Scene(const Scene &that) = delete;
    Scene& operator=(const Scene &that) = delete;

    void DrawModels(Shader *p_shader, bool p_textured);
    void SortModels(Camera *p_camera);
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
    ~Scene();

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

    bool HasShadows() const;
    void SetShadows(bool p_state);
    void GetShadowsArea(glm::vec2 &p_area) const;
    void SetShadowsArea(const glm::vec2 &p_area);
    void GetShadowsDepth(glm::vec2 &p_depth) const;
    void SetShadowsDepth(const glm::vec2 &p_depth);
    int GetShadowsQuality() const;
    void SetShadowsQuality(int p_quality);

    bool Draw(float p_time, const glm::ivec4 &p_viewport);
};

}

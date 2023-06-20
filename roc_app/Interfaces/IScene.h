#pragma once

namespace ROC
{

class ICamera;
class ILight;
class IModel;
class IRenderTarget;
class IShader;

class IScene
{
protected:
    ~IScene() = default;
public:
    virtual bool SetICamera(ICamera *p_cam) = 0;
    virtual bool RemoveCamera() = 0;
    virtual ICamera* GetICamera() const = 0;
    virtual bool SetIRenderTarget(IRenderTarget *p_rt) = 0;
    virtual bool RemoveRenderTarget() = 0;
    virtual IRenderTarget* GetIRenderTarget() const = 0;
    virtual bool SetIShader(IShader *p_shader) = 0;
    virtual bool RemoveIShader() = 0;
    virtual IShader* GetIShader() const = 0;
    virtual bool AddILight(ILight *p_light) = 0;
    virtual bool RemoveILight(ILight *p_light) = 0;
    virtual bool HasILight(ILight *p_light) const = 0;
    virtual bool AddIModel(IModel *p_model) = 0;
    virtual bool RemoveIModel(IModel *p_model) = 0;
    virtual bool HasIModel(IModel *p_model) const = 0;
    virtual bool HasShadows() const = 0;
    virtual void SetShadows(bool p_state) = 0;
    virtual void GetShadowsArea(glm::vec2 &p_area) const = 0;
    virtual void SetShadowsArea(const glm::vec2 &p_area) = 0;
    virtual void GetShadowsDepth(glm::vec2 &p_depth) const = 0;
    virtual void SetShadowsDepth(const glm::vec2 &p_depth) = 0;
    virtual int GetShadowsQuality() const = 0;
    virtual void SetShadowsQuality(int p_quality) = 0;
};

}

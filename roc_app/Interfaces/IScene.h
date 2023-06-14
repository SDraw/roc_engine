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
public:
    virtual bool IsActive() const = 0;
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
};

}

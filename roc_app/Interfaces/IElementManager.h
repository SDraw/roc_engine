#pragma once

namespace ROC
{

class IElement;
class IAnimation;
class ICamera;
class ICollision;
class IFont;
class ILight;
class IMesh;
class IModel;
class IScene;
class IRenderTarget;
class IShader;
class ISound;
class ITexture;

class IElementManager
{
public:
    virtual IScene* CreateIScene() = 0;
    virtual ICamera* CreateICamera(unsigned char p_type) = 0;
    virtual ILight* CreateILight(unsigned char p_type) = 0;
    virtual IMesh* CreateIMesh(const char *p_path) = 0;
    virtual IModel* CreateIModel(IMesh *p_mesh) = 0;
    virtual IShader* CreateIShader(const char *p_vpath, const char *p_fpath, const char *p_gpath) = 0;
    virtual IAnimation* CreateIAnimation(const char *p_path) = 0;
    virtual ISound* CreateISound(const char *p_path) = 0;
    virtual IRenderTarget* CreateIRenderTarget(unsigned char p_type, const glm::ivec2 &p_size, unsigned char p_filter) = 0;
    virtual ITexture* CreateITexture(const char *p_path, unsigned char p_type, unsigned char p_filter, bool p_compress) = 0;
    virtual IFont* CreateIFont(const char *p_path, int p_size, const glm::ivec2 &p_atlas, unsigned char p_filter) = 0;
    virtual ICollision* CreateICollision(unsigned char p_type, const glm::vec3 &p_size, float p_mass) = 0;
    virtual bool IsValidIElement(IElement *p_ptr) const = 0;
    virtual bool DestroyIElement(IElement *p_element) = 0;
};

}

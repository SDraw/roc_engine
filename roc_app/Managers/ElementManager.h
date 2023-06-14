#pragma once
#include "Interfaces/IElementManager.h"
#include "Managers/Manager.h"

#ifdef CreateFont
#undef CreateFont
#endif

namespace ROC
{

class Element;
class Animation;
class Camera;
class Collision;
class Font;
class Light;
class Mesh;
class Model;
class Scene;
class RenderTarget;
class Shader;
class Sound;
class Texture;

class ElementManager final : public IElementManager, public Manager
{
    std::unordered_set<Element*> m_elements;
    std::unordered_set<IElement*> m_interfaces;
    std::unordered_set<Element*>::iterator m_elementsEnd;
    std::unordered_set<IElement*>::iterator m_interfacesEnd;

    void RemoveElementFromSet(Element *p_ptr);

    ElementManager(const ElementManager &that) = delete;
    ElementManager& operator=(const ElementManager &that) = delete;

    //ROC::IElementManager
    IScene* CreateIScene();
    ICamera* CreateICamera(unsigned char p_type);
    ILight* CreateILight(unsigned char p_type);
    IMesh* CreateIMesh(const char *p_path);
    IModel* CreateIModel(IMesh *p_mesh);
    IShader* CreateIShader(const char *p_vpath, const char *p_fpath, const char *p_gpath);
    IAnimation* CreateIAnimation(const char *p_path);
    ISound* CreateISound(const char *p_path);
    IRenderTarget* CreateIRenderTarget(unsigned char p_type, const glm::ivec2 &p_size, unsigned char p_filter);
    ITexture* CreateITexture(const char *p_path, unsigned char p_type, unsigned char p_filter, bool p_compress);
    IFont* CreateIFont(const char *p_path, int p_size, const glm::ivec2 &p_atlas, unsigned char p_filter);
    ICollision* CreateICollision(unsigned char p_type, const glm::vec3 &p_size, float p_mass);
    bool IsValidIElement(IElement *p_ptr) const;
    bool DestroyIElement(IElement *p_element);
public:
    explicit ElementManager(Core *p_core);
    ~ElementManager() = default;

    // ROC::Manager
    void Stop() override;

    Scene* CreateScene();
    Camera* CreateCamera(unsigned char p_type);
    Light* CreateLight(unsigned char p_type);
    Mesh* CreateMesh(const std::string &p_path);
    Model* CreateModel(Mesh *p_mesh);
    Shader* CreateShader(const std::string &p_vpath, const std::string &p_fpath, const std::string &p_gpath);
    Animation* CreateAnimation(const std::string &p_path);
    Sound* CreateSound(const std::string &p_path);
    RenderTarget* CreateRenderTarget(unsigned char p_type, const glm::ivec2 &p_size, unsigned char p_filter);
    Texture* CreateTexture(const std::string &p_path, unsigned char p_type, unsigned char p_filter, bool p_compress);
    Texture* ROC::ElementManager::CreateTexture(const std::vector<std::string> &p_path, unsigned char p_filter, bool p_compress);
    Font* CreateFont(const std::string &p_path, int p_size, const glm::ivec2 &p_atlas, unsigned char p_filter);
    Collision* CreateCollision(unsigned char p_type, const glm::vec3 &p_size, float p_mass);

    bool IsValidElement(Element *p_ptr) const;
    bool DestroyElement(Element *p_element);

    // AsyncManager only call
    void AddElementToSet(Element *p_ptr);
};

}

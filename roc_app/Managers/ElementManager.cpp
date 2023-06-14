#include "stdafx.h"

#include "Managers/ElementManager.h"
#include "Core/Core.h"
#include "Elements/Animation/Animation.h"
#include "Elements/Camera.h"
#include "Elements/Collision.h"
#include "Elements/Font.h"
#include "Elements/Mesh/Mesh.h"
#include "Elements/Light.h"
#include "Elements/Model/Model.h"
#include "Elements/Scene/Scene.h"
#include "Elements/RenderTarget.h"
#include "Elements/Shader/Shader.h"
#include "Elements/Sound.h"
#include "Elements/Texture.h"

#include "Managers/LogManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/PreRenderManager.h"
#include "Managers/RenderManager/RenderManager.h"

ROC::ElementManager::ElementManager(Core *p_core) : Manager(p_core)
{
    m_elementsEnd = m_elements.end();
    m_interfacesEnd = m_interfaces.end();
}

void ROC::ElementManager::Stop()
{
    if(IsActive())
    {
        for(auto l_element : m_elements) delete l_element;
        m_elements.clear();
        m_interfaces.clear();
    }

    Manager::Stop();
}

void ROC::ElementManager::AddElementToSet(Element *p_ptr)
{
    if(!IsActive()) return;

    m_elements.insert(p_ptr);
    m_interfaces.insert(p_ptr);
    m_elementsEnd = m_elements.end();
    m_interfacesEnd = m_interfaces.end();
}

void ROC::ElementManager::RemoveElementFromSet(Element *p_ptr)
{
    m_elements.erase(p_ptr);
    m_interfaces.erase(p_ptr);
    m_elementsEnd = m_elements.end();
    m_interfacesEnd = m_interfaces.end();
}

ROC::Scene* ROC::ElementManager::CreateScene()
{
    if(!IsActive()) return nullptr;

    ROC::Scene *l_scene = new Scene();
    AddElementToSet(l_scene);
    return l_scene;
}

ROC::Camera* ROC::ElementManager::CreateCamera(unsigned char p_type)
{
    if(!IsActive()) return nullptr;

    Camera *l_camera = new Camera(p_type);
    AddElementToSet(l_camera);
    return l_camera;
}

ROC::Light* ROC::ElementManager::CreateLight(unsigned char p_type)
{
    if(!IsActive()) return nullptr;

    Light *l_light = new Light(p_type);
    AddElementToSet(l_light);
    return l_light;
}

ROC::Animation* ROC::ElementManager::CreateAnimation(const std::string &p_path)
{
    if(!IsActive()) return nullptr;

    Animation *l_anim = new Animation();
    if(l_anim->Load(p_path)) AddElementToSet(l_anim);
    else
    {
        delete l_anim;
        l_anim = nullptr;
    }
    return l_anim;
}

ROC::Mesh* ROC::ElementManager::CreateMesh(const std::string &p_path)
{
    if(!IsActive()) return nullptr;

    Mesh *l_mesh = new Mesh();
    if(l_mesh->Load(p_path))
    {
        l_mesh->GenerateMaterials();
        AddElementToSet(l_mesh);
    }
    else
    {
        delete l_mesh;
        l_mesh = nullptr;
    }
    return l_mesh;
}

ROC::Model* ROC::ElementManager::CreateModel(Mesh *p_mesh)
{
    if(!IsActive()) return nullptr;

    Model *l_model = new Model(p_mesh);
    AddElementToSet(l_model);
    GetCore()->GetPreRenderManager()->AddModel(l_model);
    GetCore()->GetPhysicsManager()->AddModel(l_model);
    return l_model;
}

ROC::Shader* ROC::ElementManager::CreateShader(const std::string &p_vpath, const std::string &p_fpath, const std::string &p_gpath)
{
    if(!IsActive()) return nullptr;

    Shader *l_shader = new Shader();
    if(l_shader->Load(p_vpath, p_fpath, p_gpath)) AddElementToSet(l_shader);
    else
    {
        const std::string &l_shaderError = l_shader->GetError();
        if(!l_shaderError.empty())
        {
            std::string l_error("[");
            l_error.append(p_vpath);
            l_error.push_back(',');
            l_error.append(p_fpath);
            l_error.push_back(',');
            l_error.append(p_gpath);
            l_error.append("] -> ");
            l_error.append(l_shaderError);
            GetCore()->GetLogManager()->Log(l_error.c_str());
        }
        delete l_shader;
        l_shader = nullptr;
    }
    return l_shader;
}

ROC::Sound* ROC::ElementManager::CreateSound(const std::string &p_path)
{
    if(!IsActive()) return nullptr;

    Sound *l_sound = new Sound();
    if(l_sound->Load(p_path)) AddElementToSet(l_sound);
    else
    {
        delete l_sound;
        l_sound = nullptr;
    }
    return l_sound;
}

ROC::RenderTarget* ROC::ElementManager::CreateRenderTarget(unsigned char p_type, const glm::ivec2 &p_size, unsigned char p_filter)
{
    if(!IsActive()) return nullptr;

    RenderTarget *l_rt = new RenderTarget();
    if(l_rt->Create(p_type, p_size, p_filter)) AddElementToSet(l_rt);
    else
    {
        delete l_rt;
        l_rt = nullptr;
    }
    return l_rt;
}

ROC::Texture* ROC::ElementManager::CreateTexture(const std::string &p_path, unsigned char p_type, unsigned char p_filter, bool p_compress)
{
    if(!IsActive()) return nullptr;

    Texture *l_texture = new Texture();
    if(l_texture->Load(p_path, p_type, p_filter, p_compress)) AddElementToSet(l_texture);
    else
    {
        delete l_texture;
        l_texture = nullptr;
    }
    return l_texture;
}

ROC::Texture* ROC::ElementManager::CreateTexture(const std::vector<std::string> &p_path, unsigned char p_filter, bool p_compress)
{
    if(!IsActive()) return nullptr;

    Texture *l_texture = new Texture();
    if(l_texture->LoadCubemap(p_path, p_filter, p_compress)) AddElementToSet(l_texture);
    else
    {
        delete l_texture;
        l_texture = nullptr;
    }
    return l_texture;
}

ROC::Font* ROC::ElementManager::CreateFont(const std::string &p_path, int p_size, const glm::ivec2 &p_atlas, unsigned char p_filter)
{
    if(!IsActive()) return nullptr;

    Font *l_font = new Font();
    if(l_font->Load(p_path, p_size, p_atlas, p_filter)) AddElementToSet(l_font);
    else
    {
        delete l_font;
        l_font = nullptr;
    }
    return l_font;
}

ROC::Collision* ROC::ElementManager::CreateCollision(unsigned char p_type, const glm::vec3 &p_size, float p_mass)
{
    if(!IsActive()) return nullptr;

    Collision *l_col = new Collision();
    if(l_col->Create(p_type, p_size, p_mass))
    {
        AddElementToSet(l_col);
        GetCore()->GetPhysicsManager()->AddCollision(l_col);
    }
    else
    {
        delete l_col;
        l_col = nullptr;
    }
    return l_col;
}

bool ROC::ElementManager::IsValidElement(Element *p_ptr) const
{
    if(!IsActive()) return false;

    auto l_checkIterator = m_elements.find(p_ptr);
    return (l_checkIterator != m_elementsEnd);
}

bool ROC::ElementManager::DestroyElement(Element *p_element)
{
    if(!IsActive()) return false;

    bool l_result = false;
    if(IsValidElement(p_element))
    {
        switch(p_element->GetElementType())
        {
            case Element::ET_Scene:
                GetCore()->GetRenderManager()->RemoveAsActiveScene(reinterpret_cast<Scene*>(p_element));
                break;
            case Element::ET_Model:
            {
                GetCore()->GetPreRenderManager()->RemoveModel(reinterpret_cast<Model*>(p_element));
                GetCore()->GetPhysicsManager()->RemoveModel(reinterpret_cast<Model*>(p_element));
            } break;
        }

        RemoveElementFromSet(p_element);
        delete p_element;
        l_result = true;
    }
    return l_result;
}

// ROC::IElementManager
bool ROC::ElementManager::IsValidIElement(IElement *p_ptr) const
{
    if(!IsActive()) return false;

    auto l_checkIterator = m_interfaces.find(p_ptr);
    return (l_checkIterator != m_interfacesEnd);
}

bool ROC::ElementManager::DestroyIElement(IElement *p_element)
{
    if(!IsActive()) return false;

    bool l_result = false;
    auto l_checkIterator = m_interfaces.find(p_element);
    if(l_checkIterator != m_interfacesEnd) l_result = DestroyElement(dynamic_cast<Element*>(p_element));
    return l_result;
}

ROC::IScene* ROC::ElementManager::CreateIScene()
{
    return CreateScene();
}

ROC::ICamera* ROC::ElementManager::CreateICamera(unsigned char p_type)
{
    return CreateCamera(p_type);
}

ROC::ILight* ROC::ElementManager::CreateILight(unsigned char p_type)
{
    return CreateLight(p_type);
}

ROC::IMesh* ROC::ElementManager::CreateIMesh(const char *p_path)
{
    return CreateMesh(p_path);
}

ROC::IModel* ROC::ElementManager::CreateIModel(IMesh *p_mesh)
{
    return CreateModel(dynamic_cast<Mesh*>(p_mesh));
}

ROC::IShader* ROC::ElementManager::CreateIShader(const char *p_vpath, const char *p_fpath, const char *p_gpath)
{
    return CreateShader(p_vpath, p_fpath, p_gpath);
}

ROC::IAnimation* ROC::ElementManager::CreateIAnimation(const char *p_path)
{
    return CreateAnimation(p_path);
}

ROC::ISound* ROC::ElementManager::CreateISound(const char *p_path)
{
    return CreateSound(p_path);
}

ROC::IRenderTarget* ROC::ElementManager::CreateIRenderTarget(unsigned char p_type, const glm::ivec2 &p_size, unsigned char p_filter)
{
    return CreateRenderTarget(p_type, p_size, p_filter);
}

ROC::ITexture* ROC::ElementManager::CreateITexture(const char *p_path, unsigned char p_type, unsigned char p_filter, bool p_compress)
{
    return CreateTexture(p_path, p_type, p_filter, p_compress);
}

ROC::IFont* ROC::ElementManager::CreateIFont(const char *p_path, int p_size, const glm::ivec2 &p_atlas, unsigned char p_filter)
{
    return CreateFont(p_path, p_size, p_atlas, p_filter);
}

ROC::ICollision* ROC::ElementManager::CreateICollision(unsigned char p_type, const glm::vec3 &p_size, float p_mass)
{
    return CreateCollision(p_type, p_size, p_mass);
}

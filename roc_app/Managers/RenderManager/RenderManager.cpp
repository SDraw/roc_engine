#include "stdafx.h"

#include "Managers/RenderManager/RenderManager.h"
#include "Core/Core.h"
#include "Managers/RenderManager/Quad2D.h"
#include "Managers/RenderManager/Quad3D.h"
#include "Managers/RenderManager/PhysicsDrawer.h"
#include "Elements/Camera.h"
#include "Elements/Font.h"
#include "Elements/Scene/Scene.h"
#include "Elements/Texture.h"
#include "Utils/CustomArguments.h"

#include "Managers/ConfigManager.h"
#include "Managers/LogManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/SfmlManager.h"
#include "Managers/ModuleManager.h"
#include "Elements/Mesh/Material.h"
#include "Elements/RenderTarget.h"
#include "Elements/Shader/Shader.h"
#include "Interfaces/IModule.h"
#include "GL/GLViewport.h"
#include "GL/GLSetting.h"
#include "GL/GLState.h"
#include "GL/GLTexture.h"

namespace ROC
{

extern const glm::mat4 g_identityMatrix;
extern const glm::vec4 g_emptyVec4;

const btVector3 g_textureZAxis(0.f, 0.f, 1.f);

}

ROC::RenderManager::RenderManager(Core *p_core) : Manager(p_core)
{
    m_renderStage = RS_None;
    m_quad2D = nullptr;
    m_quad3D = nullptr;
    m_physicsDrawer = nullptr;
    m_screenCamera = nullptr;
    m_time = 0.f;
    m_arguments = new CustomArguments();
}

ROC::RenderManager::~RenderManager()
{
    delete m_arguments;
}

// ROC::Manager
void ROC::RenderManager::Start()
{
    if(!IsActive())
    {
        GLSetting::Set(GL_DEPTH_TEST, true);
        GLSetting::Set(GL_TEXTURE_CUBE_MAP_SEAMLESS, true);
        GLSetting::Set(GL_CULL_FACE, true); // default culling
        GLSetting::SetDepthMask(true);
        GLSetting::SetPixelUnpackAlignment(1);
        if(GetCore()->GetConfigManager()->GetAntialiasing() > 0) GLSetting::Set(GL_LINE_SMOOTH, true);

        GLViewport::SetClearColor(0.223529f, 0.223529f, 0.223529f, 0.f);
        GLViewport::SetBlendFunctions(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Font::InitStaticResources();
        Shader::InitStaticResources();
        Material::InitStaticResources();

        m_quad2D = new Quad2D();
        m_quad3D = new Quad3D();
        m_physicsDrawer = new PhysicsDrawer();
        m_screenCamera = new Camera(Camera::CPT_Screen);
        GetCore()->GetPhysicsManager()->SetDebugDrawer(m_physicsDrawer);

        glm::ivec2 l_windowSize;
        GetCore()->GetSfmlManager()->GetWindowSize(l_windowSize);
        m_screenCamera->SetOrthoParams(glm::vec4(0.f, l_windowSize.x, 0.f, l_windowSize.y));
        m_screenCamera->Update();
    }

    Manager::Start();
}

void ROC::RenderManager::Stop()
{
    if(IsActive())
    {
        delete m_quad2D;
        delete m_quad3D;
        delete m_physicsDrawer;
        delete m_screenCamera;
        Font::ReleaseStaticResources();
        Material::ReleaseStaticResources();
        Shader::ReleaseStaticResources();
    }

    Manager::Stop();
}

void ROC::RenderManager::UpdateScreenSize(const glm::ivec2 &p_size)
{
    if(!IsActive()) return;

    const glm::vec4 l_params(0.f, p_size.x, 0.f, p_size.y);
    m_screenCamera->SetOrthoParams(l_params);
    m_screenCamera->Update();
}

bool ROC::RenderManager::Draw(Scene *p_scene)
{
    if(!IsActive()) return false;

    bool l_result = false;
    if(m_renderStage != RS_None)
    {
        const glm::ivec4 l_params = m_screenCamera->GetOrthoParams();
        l_result = p_scene->Draw(m_time, l_params);
        GLViewport::SetViewport(l_params.x, l_params.z, l_params.y, l_params.w);
    }
    return l_result;
}

bool ROC::RenderManager::Draw(Font *p_font, const glm::vec2 &p_pos, const std::string &p_text, const glm::vec4 &p_color)
{
    if(!IsActive()) return false;

    bool l_result = false;
    if(m_renderStage != RS_None)
    {
        Shader::GetScreenShader()->Enable();
        Shader::GetScreenShader()->SetProjectionMatrix(m_screenCamera->GetProjectionMatrix());
        Shader::GetScreenShader()->SetModelMatrix(g_identityMatrix);
        Shader::GetScreenShader()->SetColor(p_color);

        sf::String l_textUtf8 = sf::String::fromUtf8(p_text.begin(), p_text.end());
        p_font->Draw(l_textUtf8, p_pos);

        Shader::GetScreenShader()->Disable();

        l_result = true;
    }
    return l_result;
}

bool ROC::RenderManager::Draw(Drawable *p_drawable, const glm::vec2 &p_pos, const glm::vec2 &p_size, float p_rot, const glm::vec4 &p_color)
{
    if(!IsActive()) return false;

    bool l_result = false;
    if(m_renderStage != RS_None)
    {
        Shader::GetScreenShader()->Enable();
        Shader::GetScreenShader()->SetProjectionMatrix(m_screenCamera->GetProjectionMatrix());

        btTransform l_textureTransform;
        btVector3 l_textureTranslate(p_pos.x, p_pos.y, 0.f);
        l_textureTransform.setIdentity();
        l_textureTransform.setOrigin(l_textureTranslate);
        if(p_rot != 0.f)
        {
            btQuaternion l_textureRotation;
            l_textureRotation.setRotation(g_textureZAxis, p_rot);
            l_textureTransform.setRotation(l_textureRotation);
        }
        glm::mat4 l_textureMatrix;
        l_textureTransform.getOpenGLMatrix(glm::value_ptr(l_textureMatrix));
        Shader::GetScreenShader()->SetModelMatrix(l_textureMatrix);
        Shader::GetScreenShader()->SetColor(p_color);

        GLSetting::Set(GL_BLEND, p_drawable->IsTransparent());
        p_drawable->Bind();
        m_quad2D->SetTransformation(p_size);
        m_quad2D->Draw();

        Shader::GetScreenShader()->Disable();

        l_result = true;
    }
    return l_result;
}

bool ROC::RenderManager::SetViewport(const glm::ivec4 &p_area)
{
    if(!IsActive()) return false;

    if(m_renderStage != RS_None) GLViewport::SetViewport(p_area.x, p_area.y, p_area.z, p_area.w);
    return (m_renderStage != RS_None);
}

bool ROC::RenderManager::ClearViewport(bool p_depth, bool p_color)
{
    if(!IsActive()) return false;

    if(m_renderStage != RS_None)
    {
        if(p_depth) GLSetting::SetDepthMask(true);
        GLViewport::Clear(p_depth, p_color);
    }
    return (m_renderStage != RS_None);
}

bool ROC::RenderManager::SetClearColor(const glm::vec4 &p_color)
{
    if(!IsActive()) return false;

    if(m_renderStage != RS_None) GLViewport::SetClearColor(p_color.r, p_color.g, p_color.b, p_color.a);
    return (m_renderStage != RS_None);
}

bool ROC::RenderManager::SetPolygonMode(int p_mode)
{
    if(!IsActive()) return false;

    if(m_renderStage != RS_None) GLSetting::SetFillMode(GL_POINT + p_mode);
    return (m_renderStage != RS_None);
}

void ROC::RenderManager::DoPulse()
{
    if(!IsActive()) return;

    m_time = GetCore()->GetSfmlManager()->GetTime();
    m_renderStage = RS_Main;

    // Clear viewport
    const glm::ivec4 l_params = m_screenCamera->GetOrthoParams();
    GLViewport::SetViewport(l_params.x, l_params.z, l_params.y, l_params.w);
    ClearViewport();

    GetCore()->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnRender, m_arguments);
    GetCore()->GetSfmlManager()->UpdateWindow();

    m_renderStage = RS_None;
}

// ROC::IRenderManager
bool ROC::RenderManager::Draw(IScene *p_scene)
{
    return Draw(dynamic_cast<Scene*>(p_scene));
}

bool ROC::RenderManager::Draw(IFont *p_font, const glm::vec2 &p_pos, const char* p_text, const glm::vec4 &p_color)
{
    std::string l_text(p_text);
    return Draw(dynamic_cast<Font*>(p_font), p_pos, l_text, p_color);
}

bool ROC::RenderManager::Draw(IDrawable *p_drawable, const glm::vec2 &p_pos, const glm::vec2 &p_size, float p_rot, const glm::vec4 &p_color)
{
    return Draw(dynamic_cast<Drawable*>(p_drawable), p_pos, p_size, p_rot, p_color);
}

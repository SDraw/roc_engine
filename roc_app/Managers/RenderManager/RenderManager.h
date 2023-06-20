#pragma once
#include "Interfaces/IRenderManager.h"
#include "Managers/Manager.h"

namespace ROC
{

class Drawable;
class Camera;
class Font;
class Scene;
class PhysicsDrawer;
class Quad2D;
class Quad3D;
class CustomArguments;

class RenderManager final : public IRenderManager, public Manager
{
    enum RenderStage : unsigned char
    {
        RS_None = 0U,
        RS_Main
    };

    RenderStage m_renderStage;
    float m_time;

    Quad2D *m_quad2D;
    Quad3D *m_quad3D;
    PhysicsDrawer *m_physicsDrawer;
    Camera *m_screenCamera;

    CustomArguments *m_arguments;

    RenderManager(const RenderManager &that) = delete;
    RenderManager& operator=(const RenderManager &that) = delete;

    // ROC::IRenderManager
    bool Draw(IScene *p_scene);
    bool Draw(IFont *p_font, const glm::vec2 &p_pos, const char* p_text, const glm::vec4 &p_color);
    bool Draw(IDrawable *p_drawable, const glm::vec2 &p_pos, const glm::vec2 &p_size, float p_rot, const glm::vec4 &p_color);
public:
    explicit RenderManager(Core *p_core);
    ~RenderManager();

    // ROC::Manager
    void Start() override;
    void Stop() override;

    bool SetActiveScene(Scene *p_scene);

    bool Draw(Scene *p_scene);
    bool Draw(Font *p_font, const glm::vec2 &p_pos, const std::string &p_text, const glm::vec4 &p_color);
    bool Draw(Drawable *p_drawable, const glm::vec2 &p_pos, const glm::vec2 &p_size, float p_rot, const glm::vec4 &p_color);

    bool SetViewport(const glm::ivec4 &p_area);
    bool ClearViewport(bool p_depth = true, bool p_color = true);
    bool SetClearColor(const glm::vec4 &p_color);
    bool SetPolygonMode(int p_mode);

    void UpdateScreenSize(const glm::ivec2 &p_size);

    void DoPulse();
};

}

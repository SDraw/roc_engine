#pragma once

namespace ROC
{

class IDrawable;
class IFont;
class IScene;

class IRenderManager
{
public:
    virtual bool SetActiveIScene(IScene *p_scene) = 0;
    virtual bool Draw(IScene *p_scene) = 0;
    virtual bool Draw(IFont *p_font, const glm::vec2 &p_pos, const std::string &p_text, const glm::vec4 &p_color) = 0;
    virtual bool Draw(IDrawable *p_drawable, const glm::vec2 &p_pos, const glm::vec2 &p_size, float p_rot, const glm::vec4 &p_color) = 0;
    virtual bool Draw(IDrawable *p_drawable, const glm::vec3 &p_pos, const glm::quat &p_rot, const glm::vec2 &p_size, const glm::bvec4 &p_params) = 0;
    virtual bool DrawPhysics(float p_width) = 0;
    virtual bool SetViewport(const glm::ivec4 &p_area) = 0;
    virtual bool ClearViewport(bool p_depth = true, bool p_color = true) = 0;
    virtual bool SetClearColor(const glm::vec4 &p_color) = 0;
    virtual bool SetPolygonMode(int p_mode) = 0;
};

}

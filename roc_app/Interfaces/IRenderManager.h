#pragma once

namespace ROC
{

class IDrawable;
class IFont;
class IScene;

class IRenderManager
{
protected:
    ~IRenderManager() = default;
public:
    virtual bool Draw(IScene *p_scene) = 0;
    virtual bool Draw(IFont *p_font, const glm::vec2 &p_pos, const char* p_text, const glm::vec4 &p_color) = 0;
    virtual bool Draw(IDrawable *p_drawable, const glm::vec2 &p_pos, const glm::vec2 &p_size, float p_rot, const glm::vec4 &p_color) = 0;
    virtual bool SetViewport(const glm::ivec4 &p_area) = 0;
    virtual bool ClearViewport(bool p_depth = true, bool p_color = true) = 0;
    virtual bool SetClearColor(const glm::vec4 &p_color) = 0;
    virtual bool SetPolygonMode(int p_mode) = 0;
};

}

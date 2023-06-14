#pragma once

class GLViewport final
{
    __declspec(thread) static GLint ms_positionX;
    __declspec(thread) static GLint ms_positionY;
    __declspec(thread) static GLsizei ms_width;
    __declspec(thread) static GLsizei ms_height;
    __declspec(thread) static float ms_clearColorR;
    __declspec(thread) static float ms_clearColorG;
    __declspec(thread) static float ms_clearColorB;
    __declspec(thread) static float ms_clearColorA;
    __declspec(thread) static GLenum ms_blendSource;
    __declspec(thread) static GLenum ms_blendDestination;
    __declspec(thread) static float ms_lineWidth;

    GLViewport() = delete;
    GLViewport(const GLViewport &that) = delete;
    GLViewport& operator=(const GLViewport &that) = delete;
    ~GLViewport() = delete;
public:
    static void SetViewport(GLint p_x, GLint p_y, GLsizei p_width, GLsizei p_height);
    static void Clear(bool p_depth, bool p_color);
    static void SetClearColor(float p_red, float p_green, float p_blue, float p_alpha);
    static void SetBlendFunctions(GLenum p_src, GLenum p_dst);
    static void SetLineWidth(float p_width);
};

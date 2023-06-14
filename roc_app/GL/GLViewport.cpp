#include "stdafx.h"

#include "GL/GLViewport.h"

GLint GLViewport::ms_positionX = 0;
GLint GLViewport::ms_positionY = 0;
GLsizei GLViewport::ms_width = 0;
GLsizei GLViewport::ms_height = 0;
float GLViewport::ms_clearColorR = 0.f;
float GLViewport::ms_clearColorG = 0.f;
float GLViewport::ms_clearColorB = 0.f;
float GLViewport::ms_clearColorA = 0.f;
GLenum GLViewport::ms_blendSource = GL_ONE;
GLenum GLViewport::ms_blendDestination = GL_ZERO;
float GLViewport::ms_lineWidth = 1.f;

void GLViewport::SetViewport(GLint p_x, GLint p_y, GLsizei p_width, GLsizei p_height)
{
    bool l_update = false;
    if(ms_positionX != p_x)
    {
        ms_positionX = p_x;
        l_update = true;
    }
    if(ms_positionY != p_y)
    {
        ms_positionY = p_y;
        l_update = true;
    }
    if(ms_width != p_width)
    {
        ms_width = p_width;
        l_update = true;
    }
    if(ms_height != p_height)
    {
        ms_height = p_height;
        l_update = true;
    }

    if(l_update) glViewport(ms_positionX, ms_positionY, ms_width, ms_height);
}

void GLViewport::Clear(bool p_depth, bool p_color)
{
    GLenum l_mode = 0;
    if(p_depth) l_mode |= GL_DEPTH_BUFFER_BIT;
    if(p_color) l_mode |= GL_COLOR_BUFFER_BIT;
    if(l_mode) glClear(l_mode);
}

void GLViewport::SetClearColor(float p_red, float p_green, float p_blue, float p_alpha)
{
    bool l_update = false;
    if(ms_clearColorR != p_red)
    {
        ms_clearColorR = p_red;
        l_update = true;
    }
    if(ms_clearColorG != p_green)
    {
        ms_clearColorG = p_green;
        l_update = true;
    }
    if(ms_clearColorB != p_blue)
    {
        ms_clearColorB = p_blue;
        l_update = true;
    }
    if(ms_clearColorA != p_alpha)
    {
        ms_clearColorA = p_alpha;
        l_update = true;
    }
    if(l_update) glClearColor(ms_clearColorR, ms_clearColorG, ms_clearColorB, ms_clearColorA);
}

void GLViewport::SetBlendFunctions(GLenum p_src, GLenum p_dst)
{
    bool l_update = false;
    if(ms_blendSource != p_src)
    {
        ms_blendSource = p_src;
        l_update = true;
    }
    if(ms_blendDestination != p_dst)
    {
        ms_blendDestination = p_dst;
        l_update = true;
    }
    if(l_update) glBlendFunc(ms_blendSource, ms_blendDestination);
}

void GLViewport::SetLineWidth(float p_width)
{
    if(ms_lineWidth != p_width)
    {
        ms_lineWidth = p_width;
        glLineWidth(ms_lineWidth);
    }
}
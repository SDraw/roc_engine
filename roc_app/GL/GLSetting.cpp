#include "stdafx.h"

#include "GL/GLSetting.h"

std::map<GLenum, bool> *GLSetting::ms_settings = nullptr;
bool GLSetting::ms_depthMask = false;
GLenum GLSetting::ms_fillMode = GL_FILL;
GLint GLSetting::ms_pixelUnpackAlignment = 4;

void GLSetting::Set(GLenum p_setting, bool p_state)
{
    if(!ms_settings) ms_settings = new std::map<GLenum, bool>();
    auto l_setting = ms_settings->find(p_setting);
    if(l_setting != ms_settings->end())
    {
        if(l_setting->second != p_state)
        {
            l_setting->second = p_state;
            p_state ? glEnable(p_setting) : glDisable(p_setting);
        }
    }
    else
    {
        ms_settings->emplace(p_setting, p_state);
        p_state ? glEnable(p_setting) : glDisable(p_setting);
    }
}

void GLSetting::SetDepthMask(bool p_state)
{
    if(ms_depthMask != p_state)
    {
        glDepthMask(p_state ? 1U : 0U);
        ms_depthMask = p_state;
    }
}

void GLSetting::SetFillMode(GLenum p_mode)
{
    if(ms_fillMode != p_mode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, p_mode);
        ms_fillMode = p_mode;
    }
}

void GLSetting::SetPixelUnpackAlignment(GLint p_value)
{
    if(ms_pixelUnpackAlignment != p_value)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, p_value);
        ms_pixelUnpackAlignment = p_value;
    }
}

const unsigned char* GLSetting::GetString(GLenum p_value)
{
    return glGetString(p_value);
}

void GLSetting::GetInteger(GLenum p_val, GLint *p_int)
{
    glGetIntegerv(p_val, p_int);
}

void GLSetting::MemoryCleanup()
{
    delete ms_settings;
    ms_settings = nullptr;
}

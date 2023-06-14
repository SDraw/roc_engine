#pragma once

class GLSetting final
{
    __declspec(thread) static std::map<GLenum, bool> *ms_settings;
    __declspec(thread) static bool ms_depthMask;
    __declspec(thread) static GLenum ms_fillMode;
    __declspec(thread) static GLint ms_pixelUnpackAlignment;

    GLSetting() = delete;
    GLSetting(const GLSetting &that) = delete;
    GLSetting& operator=(const GLSetting &that) = delete;
    ~GLSetting() = delete;
public:
    static void Set(GLenum p_setting, bool p_state);
    static void SetDepthMask(bool p_state);
    static void SetFillMode(GLenum p_mode);
    static void SetPixelUnpackAlignment(GLint p_value);
    static const unsigned char* GetString(GLenum p_value);
    static void GetInteger(GLenum p_val, GLint *p_int);

    static void MemoryCleanup();
};

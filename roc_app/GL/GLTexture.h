#pragma once

class GLTexture
{
    GLTexture(const GLTexture &that) = delete;
    GLTexture& operator=(const GLTexture &that) = delete;
public:
    virtual ~GLTexture() = default;

    GLuint GetName() const;

    bool Destroy();
    bool Bind(GLenum p_slot = 0U);

    static void Rebind();
protected:
    GLuint m_name;
    GLenum m_internalType;

    __declspec(thread) static GLuint ms_activeSlot;
    __declspec(thread) static std::array<GLuint, 16U> ms_activeName;
    __declspec(thread) static std::array<GLenum, 16U> ms_activeNameType;

    GLTexture();
};

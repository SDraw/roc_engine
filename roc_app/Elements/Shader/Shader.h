#pragma once
#include "Interfaces/IShader.h"
#include "Elements/Element.h"

class GLShader;

namespace ROC
{

class Drawable;
class Light;
class Pool;
class ShaderUniform;

class Shader final : public Element, public IShader
{
    GLShader *m_shader;
    bool m_active;

    enum ShaderDefaultUniform : size_t
    {
        SDU_Projection,
        SDU_View,
        SDU_ViewProjection,
        SDU_Model,
        SDU_BoneMatrices,
        SDU_CameraPosition,
        SDU_CameraDirection,
        SDU_LightData,
        SDU_LightsCount,
        SDU_MaterialParam,
        SDU_MaterialType,
        SDU_Animated,
        SDU_DiffuseTexture,
        SDU_Time,
        SDU_Color,

        SDU_UniformCount
    };
    std::array<ShaderUniform*, SDU_UniformCount> m_defaultUniforms;

    std::unordered_map<std::string, ShaderUniform*> m_uniformMap;
    std::unordered_map<std::string, ShaderUniform*>::iterator m_uniformMapEnd;

    Pool *m_bindPool;
    struct DrawableBindData
    {
        Drawable *m_element;
        int m_slot;
        GLint m_uniform;
    };
    std::vector<DrawableBindData> m_drawableBind;
    static size_t ms_drawableMaxCount;

    std::string m_error;

    const static std::string ms_defaultDefines;

    Shader(const Shader &that) = delete;
    Shader& operator=(const Shader &that) = delete;

    void SetupUniformsAndLocations();
    void FindDefaultUniform(ShaderDefaultUniform p_sud, unsigned char p_type, const char *p_name, size_t p_dataSize, size_t p_count = 1U);

    ShaderUniform* GetUniform(const std::string &p_name);
    void UpdateShaderFromUniform(ShaderUniform *l_uniform);

    static size_t GetSizeFromGLType(GLenum p_type);
    static unsigned char GetTypeFromGLType(GLenum p_type);

    bool Attach(IDrawable *p_drawable, const char *p_uniform);
    bool Detach(IDrawable *p_drawable);
    bool HasAttached(IDrawable *p_drawable) const;

    // ROC::Element
    void OnParentRemoved(Element *p_parent) override;
    void OnChildRemoved(Element *p_child) override;
public:
    Shader();
    ~Shader();

    bool Attach(Drawable *p_drawable, const std::string &p_uniform);
    bool Detach(Drawable *p_drawable);
    bool HasAttached(Drawable *p_drawable) const;

    bool SetUniformValue(const char *p_name, ShaderUniformType p_type, const void *p_data, size_t p_size);

    bool Load(const std::string &p_vpath, const std::string &p_fpath, const std::string &p_gpath);
    const std::string& GetError() const;

    void SetProjectionMatrix(const glm::mat4 &p_value);
    void SetViewMatrix(const glm::mat4 &p_value);
    void SetViewProjectionMatrix(const glm::mat4 &p_value);
    void SetModelMatrix(const glm::mat4 &p_value);
    void SetBoneMatrices(const std::vector<glm::mat4> &p_value);
    void SetCameraPosition(const glm::vec3 &p_value);
    void SetCameraDirection(const glm::vec3 &p_value);
    void SetLightsData(const std::vector<Light*> &p_data);
    void SetMaterialParam(const glm::vec4 &p_value);
    void SetMaterialType(int p_value);
    void SetAnimated(bool p_value);
    void SetTime(float p_value);
    void SetColor(const glm::vec4 &p_value);

    void Enable();
    void Disable();
    bool IsActive() const;

    static void InitStaticResources();
};

}

#include "stdafx.h"
#include "Lua/LuaDefs/ShaderDefs.h"
#include "Lua/LuaDefs.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaDefs/ElementDefs.h"
#include "Lua/LuaArgReader.h"
#include "Core.h"
#include "Utils.h"

extern const std::string g_drawableName;
extern const std::string g_vec2Name;
extern const std::string g_vec3Name;
extern const std::string g_vec4Name;
extern const std::string g_matrix2Name;
extern const std::string g_matrix3Name;
extern const std::string g_matrix4Name;
extern const std::string g_shaderName("Shader");
const std::string g_shaderInvalid("Invalid object/Shader");
const std::string g_drawableInvalid("Invalid object/Drawable");

const std::vector<std::string> g_uniformTypes
{
    "float", "float2", "float3", "float4",
    "int", "int2", "int3", "int4",
    "uint", "uint2", "uint3", "uint4",
    "double", "double2", "double3", "double4",
    "mat2", "mat3", "mat4",
    "dmat2", "dmat3", "dmat4",
};

std::vector<LuaMethodDef> ShaderDefs::ms_metaMethods;
std::vector<LuaPropDef> ShaderDefs::ms_instanceProps;
std::vector<LuaMethodDef> ShaderDefs::ms_instanceMethods;

void ShaderDefs::Init()
{
    ms_instanceMethods.emplace_back("attach", Attach);
    ms_instanceMethods.emplace_back("detach", Detach);
    ms_instanceMethods.emplace_back("hasAttached", Detach);
    ms_instanceMethods.emplace_back("setUniform", SetUniform);

    ElementDefs::InheritTo(&ms_metaMethods, &ms_instanceProps);
}
void ShaderDefs::RegisterInVM(LuaVM *p_vm)
{
    p_vm->RegisterLuaClass(g_shaderName, Create, nullptr, nullptr, &ms_metaMethods, &ms_instanceProps, &ms_instanceMethods);
    p_vm->RegisterFunction("isShader", IsShader);
}

int ShaderDefs::Create(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    std::string l_fileA;
    std::string l_fileB;
    std::string l_fileC;
    l_argReader.Skip();
    l_argReader.ReadString(l_fileA);
    l_argReader.ReadString(l_fileB);
    l_argReader.ReadNextString(l_fileC);
    if(!l_argReader.HasError())
    {
        ROC::IShader *l_shader = Core::GetInstance()->GetEngineCore()->GetIElementManager()->CreateIShader(l_fileA.c_str(), l_fileB.c_str(), l_fileC.c_str());
        if(l_shader)
            l_argReader.PushObject(Utils::Cast<ROC::IElement*>(l_shader), g_shaderName, true);
        else
            l_argReader.PushBoolean(false);
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int ShaderDefs::Attach(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_shaderObj;
    std::string l_uniform;
    ROC::IElement *l_drawableObj;
    l_argReader.ReadObject(l_shaderObj, g_shaderName);
    l_argReader.ReadString(l_uniform);
    l_argReader.ReadObject(l_drawableObj);
    if(!l_argReader.HasError())
    {
        ROC::IShader *l_shader;
        if(Utils::IsValid(l_shaderObj) && (l_shader = Utils::Cast<ROC::IShader*>(l_shaderObj)))
        {
            ROC::IDrawable *l_drawable;
            if(Utils::IsValid(l_drawableObj) && (l_drawable = Utils::Cast<ROC::IDrawable*>(l_drawableObj)))
                l_argReader.PushBoolean(l_shader->Attach(l_drawable, l_uniform.c_str()));
            else
            {
                l_argReader.SetError(g_drawableInvalid);
                l_argReader.PushBoolean(false);
            }
        }
        else
        {
            l_argReader.SetError(g_shaderInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
                    l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int ShaderDefs::Detach(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_shaderObj;
    ROC::IElement *l_drawableObj;
    l_argReader.ReadObject(l_shaderObj, g_shaderName);
    l_argReader.ReadObject(l_drawableObj);
    if(!l_argReader.HasError())
    {
        ROC::IShader *l_shader;
        if(Utils::IsValid(l_shaderObj) && (l_shader = Utils::Cast<ROC::IShader*>(l_shaderObj)))
        {
            ROC::IDrawable *l_drawable;
            if(Utils::IsValid(l_drawableObj) && (l_drawable = Utils::Cast<ROC::IDrawable*>(l_drawableObj)))
                l_argReader.PushBoolean(l_shader->Detach(l_drawable));
            else
            {
                l_argReader.SetError(g_drawableInvalid);
                l_argReader.PushBoolean(false);
            }
        }
        else
        {
            l_argReader.SetError(g_shaderInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int ShaderDefs::HasAttached(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_shaderObj;
    ROC::IElement *l_drawableObj;
    l_argReader.ReadObject(l_shaderObj, g_shaderName);
    l_argReader.ReadObject(l_drawableObj);
    if(!l_argReader.HasError())
    {
        ROC::IShader *l_shader;
        if(Utils::IsValid(l_shaderObj) && (l_shader = Utils::Cast<ROC::IShader*>(l_shaderObj)))
        {
            ROC::IDrawable *l_drawable;
            if(Utils::IsValid(l_drawableObj) && (l_drawable = Utils::Cast<ROC::IDrawable*>(l_drawableObj)))
                l_argReader.PushBoolean(l_shader->HasAttached(l_drawable));
            else
            {
                l_argReader.SetError(g_drawableInvalid);
                l_argReader.PushBoolean(false);
            }
        }
        else
        {
            l_argReader.SetError(g_shaderInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}

int ShaderDefs::SetUniform(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_shaderObj;
    std::string l_uniform;
    ROC::IShader::ShaderUniformType l_uniformType;
    l_argReader.ReadObject(l_shaderObj, g_shaderName);
    l_argReader.ReadString(l_uniform);
    l_argReader.ReadEnum(l_uniformType, g_uniformTypes);
    if(!l_argReader.HasError())
    {
        ROC::IShader *l_shader;
        if(Utils::IsValid(l_shaderObj) && (l_shader = Utils::Cast<ROC::IShader*>(l_shaderObj)))
        {
            switch(l_uniformType)
            {
                case ROC::IShader::SUT_Float:
                {
                    float l_val;
                    l_argReader.ReadNumber(l_val);
                    if(!l_argReader.HasError())
                        l_argReader.PushBoolean(l_shader->SetUniformValue(l_uniform.c_str(), l_uniformType, &l_val, sizeof(float)));
                    else
                        l_argReader.PushBoolean(false);
                } break;

                case ROC::IShader::SUT_Float2:
                {
                    glm::vec2 *l_vec;
                    l_argReader.ReadObject(l_vec, g_vec2Name);
                    if(!l_argReader.HasError())
                        l_argReader.PushBoolean(l_shader->SetUniformValue(l_uniform.c_str(), l_uniformType, l_vec, sizeof(glm::vec2)));
                    else
                        l_argReader.PushBoolean(false);
                } break;

                case ROC::IShader::SUT_Float3:
                {
                    glm::vec3 *l_vec;
                    l_argReader.ReadObject(l_vec, g_vec3Name);
                    if(!l_argReader.HasError())
                        l_argReader.PushBoolean(l_shader->SetUniformValue(l_uniform.c_str(), l_uniformType, l_vec, sizeof(glm::vec3)));
                    else
                        l_argReader.PushBoolean(false);
                } break;

                case ROC::IShader::SUT_Float4:
                {
                    glm::vec4 *l_vec;
                    l_argReader.ReadObject(l_vec, g_vec4Name);
                    if(!l_argReader.HasError())
                        l_argReader.PushBoolean(l_shader->SetUniformValue(l_uniform.c_str(), l_uniformType, l_vec, sizeof(glm::vec4)));
                    else
                        l_argReader.PushBoolean(false);
                } break;

                case ROC::IShader::SUT_Int:
                {
                    int l_val;
                    l_argReader.ReadInteger(l_val);
                    if(!l_argReader.HasError())
                        l_argReader.PushBoolean(l_shader->SetUniformValue(l_uniform.c_str(), l_uniformType, &l_val, sizeof(int)));
                    else
                        l_argReader.PushBoolean(false);
                } break;

                case ROC::IShader::SUT_UInt:
                {
                    std::int64_t l_val;
                    l_argReader.ReadInteger(l_val);
                    if(!l_argReader.HasError())
                    {
                        auto l_uVal = static_cast<unsigned int>(l_val);
                        l_argReader.PushBoolean(l_shader->SetUniformValue(l_uniform.c_str(), l_uniformType, &l_uVal, sizeof(unsigned int)));
                    }
                    else
                        l_argReader.PushBoolean(false);
                } break;

                case ROC::IShader::SUT_Double:
                {
                    double l_val;
                    l_argReader.ReadNumber(l_val);
                    if(!l_argReader.HasError())
                        l_argReader.PushBoolean(l_shader->SetUniformValue(l_uniform.c_str(), l_uniformType, &l_val, sizeof(double)));
                    else
                        l_argReader.PushBoolean(false);
                } break;

                case ROC::IShader::SUT_FloatMat2:
                {
                    glm::mat2 *l_mat;
                    l_argReader.ReadObject(l_mat, g_matrix2Name);
                    if(!l_argReader.HasError())
                        l_argReader.PushBoolean(l_shader->SetUniformValue(l_uniform.c_str(), l_uniformType, l_mat, sizeof(glm::mat2)));
                    else
                        l_argReader.PushBoolean(false);
                } break;

                case ROC::IShader::SUT_FloatMat3:
                {
                    glm::mat3 *l_mat;
                    l_argReader.ReadObject(l_mat, g_matrix3Name);
                    if(!l_argReader.HasError())
                        l_argReader.PushBoolean(l_shader->SetUniformValue(l_uniform.c_str(), l_uniformType, l_mat, sizeof(glm::mat3)));
                    else
                        l_argReader.PushBoolean(false);
                } break;

                case ROC::IShader::SUT_FloatMat4:
                {
                    glm::mat4 *l_mat;
                    l_argReader.ReadObject(l_mat, g_matrix4Name);
                    if(!l_argReader.HasError())
                        l_argReader.PushBoolean(l_shader->SetUniformValue(l_uniform.c_str(), l_uniformType, l_mat, sizeof(glm::mat4)));
                    else
                        l_argReader.PushBoolean(false);
                } break;

                default:
                    l_argReader.PushBoolean(false);
                    break;
            }
        }
        else
        {
            l_argReader.SetError(g_shaderInvalid);
            l_argReader.PushBoolean(false);
        }
    }
    else
        l_argReader.PushBoolean(false);

    l_argReader.LogError();
    return l_argReader.GetReturnValue();
}


int ShaderDefs::IsShader(lua_State *p_state)
{
    LuaArgReader l_argReader(p_state);
    ROC::IElement *l_obj = nullptr;
    l_argReader.ReadNextObject(l_obj, g_shaderName);
    l_argReader.PushBoolean(l_obj && Utils::IsValid(l_obj) && Utils::Cast<ROC::IShader*>(l_obj));
    return l_argReader.GetReturnValue();
}

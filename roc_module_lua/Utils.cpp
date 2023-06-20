#include "stdafx.h"
#include "Utils.h"
#include "Core.h"

extern const std::string g_vec2Name;
extern const std::string g_vec3Name;
extern const std::string g_vec4Name;
extern const std::string g_quatName;
extern const std::string g_matrixName;

bool Utils::IsValid(ROC::IElement *p_obj)
{
    return Core::GetInstance()->GetEngineCore()->GetIElementManager()->IsValidIElement(p_obj);
}

void Utils::DeleteByType(void *p_obj, const std::string &p_type)
{
    if(p_type == g_vec2Name)
    {
        delete reinterpret_cast<glm::vec2*>(p_obj);
        return;
    }
    if(p_type == g_vec3Name)
    {
        delete reinterpret_cast<glm::vec3*>(p_obj);
        return;
    }
    if(p_type == g_vec4Name)
    {
        delete reinterpret_cast<glm::vec4*>(p_obj);
        return;
    }
    if(p_type == g_quatName)
    {
        delete reinterpret_cast<glm::quat*>(p_obj);
        return;
    }
    if(p_type == g_matrixName)
    {
        delete reinterpret_cast<glm::mat4x4*>(p_obj);
        return;
    }
}

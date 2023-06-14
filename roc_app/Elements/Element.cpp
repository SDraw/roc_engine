#include "stdafx.h"

#include "Elements/Element.h"
#include "Utils/CustomArgument.h"

namespace ROC
{

const std::array<std::string, 12U> g_elementTypeName =
{
    "Model",
    "Mesh",
    "Animation",
    "Scene",
    "Camera",
    "Light",
    "RenderTarget",
    "Shader",
    "Sound",
    "Texture",
    "Font",
    "Collision"
};
const std::string g_elementInvalidName = "Invalid";

}

ROC::Element::Element()
{
    m_elementType = ET_Invalid;
}

ROC::Element::~Element()
{
    for(auto l_child : m_children) l_child->OnParentRemoved(this);
    m_children.clear();

    for(auto l_parent : m_parents) l_parent->OnChildRemoved(this);
    m_parents.clear();
}

bool ROC::Element::HasChildren() const
{
    return !m_children.empty();
}

bool ROC::Element::HasParents() const
{
    return !m_parents.empty();
}

unsigned char ROC::Element::GetElementType() const
{
    return m_elementType;
}

bool ROC::Element::HasChild(Element *p_child)
{
    bool l_result = (std::find(m_children.begin(), m_children.end(), p_child) != m_children.end());
    return l_result;
}

bool ROC::Element::HasParent(Element *p_parent)
{
    bool l_result = (std::find(m_parents.begin(), m_parents.end(), p_parent) != m_parents.end());
    return l_result;
}

void ROC::Element::Link(Element *p_parent, Element *p_child)
{
    if(!p_parent->HasChild(p_child))
        p_parent->m_children.push_back(p_child);

    if(!p_child->HasParent(p_parent))
        p_child->m_parents.push_back(p_parent);
}

void ROC::Element::Unlink(Element *p_parent, Element *p_child)
{
    if(p_parent->HasChild(p_child))
    {
        p_child->OnParentRemoved(p_parent);
        p_parent->m_children.erase(std::find(p_parent->m_children.begin(), p_parent->m_children.end(), p_child));
    }

    if(p_child->HasParent(p_parent))
    {
        p_parent->OnChildRemoved(p_child);
        p_child->m_parents.erase(std::find(p_child->m_parents.begin(), p_child->m_parents.end(), p_parent));
    }
}

void ROC::Element::OnParentRemoved(Element *p_parent)
{
    m_parents.erase(std::find(m_parents.begin(), m_parents.end(), p_parent));
}

void ROC::Element::OnChildRemoved(Element *p_child)
{
    m_children.erase(std::find(m_children.begin(), m_children.end(), p_child));
}

// ROC::IElement
const char* ROC::Element::GetElementTypeName() const
{
    return ((m_elementType != ET_Invalid) ? g_elementTypeName[m_elementType] : g_elementInvalidName).c_str();
}
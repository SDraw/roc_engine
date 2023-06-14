#pragma once
#include "Interfaces/IElement.h"

namespace ROC
{

class Element : public IElement
{
    std::vector<Element*> m_children;
    std::vector<Element*> m_parents;

    Element(const Element &that) = delete;
    Element& operator=(const Element &that) = delete;
public:
    virtual ~Element();

    bool HasChildren() const;
    const std::vector<Element*>& GetChildren() const { return m_children; }
    bool HasChild(Element *p_child);

    bool HasParents() const;
    const std::vector<Element*>& GetParents() const { return m_parents; }
    bool HasParent(Element *p_parent);

    unsigned char GetElementType() const;
    const char* GetElementTypeName() const;
protected:
    unsigned char m_elementType;

    Element();

    // Derived classes use only
    static void Link(Element *p_parent, Element *p_child);
    static void Unlink(Element *p_parent, Element *p_child);

    // Must-call methods from derived classes
    virtual void OnParentRemoved(Element *p_parent);
    virtual void OnChildRemoved(Element *p_child);
};

}

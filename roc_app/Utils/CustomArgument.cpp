#include "stdafx.h"

#include "Utils/CustomArgument.h"

ROC::CustomArgument::CustomArgument()
{
    m_type = CAT_None;
    m_ptr = nullptr;
}

ROC::CustomArgument::CustomArgument(bool p_val)
{
    m_type = CAT_Boolean;
    m_bool = p_val;
}
ROC::CustomArgument::CustomArgument(int p_val)
{
    m_type = CAT_Integer;
    m_int = p_val;
}

ROC::CustomArgument::CustomArgument(unsigned int p_val)
{
    m_type = CAT_UInteger;
    m_uint = p_val;
}

ROC::CustomArgument::CustomArgument(float p_val)
{
    m_type = CAT_Float;
    m_float = p_val;
}

ROC::CustomArgument::CustomArgument(double p_val)
{
    m_type = CAT_Double;
    m_double = p_val;
}

ROC::CustomArgument::CustomArgument(void *p_val)
{
    m_type = CAT_Pointer;
    m_ptr = p_val;
}

ROC::CustomArgument::CustomArgument(const std::string &p_val)
{
    m_string.assign(p_val);
    m_type = CAT_String;
}

ROC::CustomArgument::CustomArgument(ROC::IElement *p_element)
{
    m_ptr = p_element;
    m_type = CAT_Element;
}

ROC::CustomArgument::CustomArgument(const CustomArgument& p_data)
{
    m_type = p_data.m_type;
    m_ptr = nullptr;
    switch(m_type)
    {
        case CAT_Boolean:
            m_bool = p_data.m_bool;
            break;
        case CAT_Integer:
            m_int = p_data.m_int;
            break;
        case CAT_UInteger:
            m_uint = p_data.m_uint;
            break;
        case CAT_Float:
            m_float = p_data.m_float;
            break;
        case CAT_Double:
            m_double = p_data.m_double;
            break;
        case CAT_Pointer: case CAT_Element:
            m_ptr = p_data.m_ptr;
            break;
        case CAT_String:
            m_string.assign(p_data.m_string);
            break;
    }
}

unsigned char ROC::CustomArgument::GetType() const { return m_type; }

bool ROC::CustomArgument::GetBoolean() const
{
    return m_bool;
}

int ROC::CustomArgument::GetInteger() const
{
    return m_int;
}

int ROC::CustomArgument::GetUInteger() const
{
    return m_uint;
}

float ROC::CustomArgument::GetFloat() const
{
    return m_float;
}

double ROC::CustomArgument::GetDouble() const
{
    return m_double;
}

void* ROC::CustomArgument::GetPointer() const
{
    return m_ptr;
}

ROC::IElement* ROC::CustomArgument::GetElement() const
{
    return reinterpret_cast<ROC::IElement*>(m_ptr);
};

const std::string& ROC::CustomArgument::GetString() const
{
    return m_string;
}

ROC::CustomArgument& ROC::CustomArgument::operator=(const ROC::CustomArgument &p_data)
{
    m_type = p_data.m_type;
    m_ptr = nullptr;
    switch(m_type)
    {
        case CAT_Boolean:
            m_bool = p_data.m_bool;
            break;
        case CAT_Integer:
            m_int = p_data.m_int;
            break;
        case CAT_UInteger:
            m_uint = p_data.m_uint;
            break;
        case CAT_Float:
            m_float = p_data.m_float;
            break;
        case CAT_Double:
            m_double = p_data.m_double;
            break;
        case CAT_Pointer: case CAT_Element:
            m_ptr = p_data.m_ptr;
            break;
        case CAT_String:
            m_string.assign(p_data.m_string);
            break;
    }
    return *this;
}

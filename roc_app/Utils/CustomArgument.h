#pragma once
#include "Interfaces/ICustomArgument.h"

namespace ROC
{

class CustomArgument final : public ICustomArgument
{
    unsigned char m_type;

    union
    {
        bool m_bool;
        int m_int;
        unsigned int m_uint;
        float m_float;
        double m_double;
        void *m_ptr;
    };
    std::string m_string;
public:
    CustomArgument();
    explicit CustomArgument(bool p_val);
    explicit CustomArgument(int p_val);
    explicit CustomArgument(unsigned int p_val);
    explicit CustomArgument(float p_val);
    explicit CustomArgument(double p_val);
    explicit CustomArgument(void *p_val);
    explicit CustomArgument(const std::string &p_val);
    explicit CustomArgument(ROC::IElement *p_element);
    CustomArgument(const CustomArgument &p_data);
    ~CustomArgument() = default;

    unsigned char GetType() const;

    bool GetBoolean() const;
    int GetInteger() const;
    int GetUInteger() const;
    float GetFloat() const;
    double GetDouble() const;
    void* GetPointer() const;
    ROC::IElement* GetElement() const;
    const char* GetString() const;

    CustomArgument& operator=(const CustomArgument &p_data);
};

}

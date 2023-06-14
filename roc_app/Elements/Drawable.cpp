#include "stdafx.h"

#include "Elements/Drawable.h"

ROC::Drawable::Drawable()
{
    m_filtering = DFT_None;
}

unsigned char ROC::Drawable::GetFiltering() const
{
    return m_filtering;
};

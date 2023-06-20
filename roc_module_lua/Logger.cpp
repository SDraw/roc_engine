#include "stdafx.h"

#include "Logger.h"
#include "Core.h"

void Logger::Log(const std::string &p_str)
{
    Core::GetInstance()->GetEngineCore()->GetILogManager()->Log(p_str.c_str());
}

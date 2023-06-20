#pragma once

class Logger final
{
    Logger() = delete;
    ~Logger() = delete;
public:
    static void Log(const std::string &p_str);
};


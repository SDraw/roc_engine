#pragma once

class GLState final
{
    GLState() = delete;
    GLState(const GLState &that) = delete;
    GLState& operator=(const GLState &that) = delete;
    ~GLState() = delete;
public:
    static void Finish();
    static void Flush();
};

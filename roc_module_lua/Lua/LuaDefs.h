#pragma once

struct LuaPropDef final
{
public:
    std::string m_name;
    lua_CFunction m_get;
    lua_CFunction m_set;

    LuaPropDef(const std::string &p_name, lua_CFunction p_get, lua_CFunction p_set)
    {
        m_name.assign(p_name);
        m_get = p_get;
        m_set = p_set;
    }
};

struct LuaMethodDef final
{
public:
    std::string m_name;
    lua_CFunction m_func;

    LuaMethodDef(const std::string &p_name, lua_CFunction p_func)
    {
        m_name.assign(p_name);
        m_func = p_func;
    }
};

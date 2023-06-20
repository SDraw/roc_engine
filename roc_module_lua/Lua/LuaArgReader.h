#pragma once

class LuaVM;
class LuaArgReader final
{
    LuaVM *m_vm;
    int m_currentArgument;
    int m_argumentsCount;
    int m_returnCount;
    bool m_hasError;
    std::string m_error;

    LuaArgReader(const LuaArgReader &that) = delete;
    LuaArgReader& operator=(const LuaArgReader &that) = delete;
public:
    LuaArgReader(lua_State *p_state);
    ~LuaArgReader() = default;

    bool HasError() const;
    void SetError(const std::string &p_error);
    void LogError() const;

    int GetReturnValue() const;

    bool IsNextNil() const;
    void PushNil();

    bool IsNextBoolean() const;
    void ReadBoolean(bool &p_val);
    void ReadNextBoolean(bool &p_val);
    void PushBoolean(bool p_val);

    bool IsNextNumber() const;
    void ReadNumber(double &p_val);
    void ReadNumber(float &p_val);
    void ReadNextNumber(double &p_val);
    void ReadNextNumber(float &p_val);
    void PushNumber(double p_val);
    void PushNumber(float p_val);

    bool IsNextInteger() const;
    void ReadInteger(std::int64_t &p_val);
    void ReadInteger(size_t &p_val);
    void ReadInteger(int &p_val);
    void ReadNextInteger(std::int64_t &p_val);
    void ReadNextInteger(size_t &p_val);
    void ReadNextInteger(int &p_val);
    void PushInteger(std::int64_t p_val);
    void PushInteger(size_t p_val);
    void PushInteger(int p_val);

    bool IsNextString() const;
    void ReadString(std::string &p_val);
    void ReadNextString(std::string &p_val);
    void PushString(const std::string &p_val);

    bool IsNextObject() const;
    template<typename T> void ReadObject(T &p_val, const std::string &p_type);
    template<typename T> void ReadObject(T &p_val);
    template<typename T> void ReadNextObject(T &p_val, const std::string &p_type);
    template<typename T> void ReadNextObject(T &p_val);
    void PushObject(void* p_val, const std::string &p_type, bool p_external);

    template<typename T> void ReadEnum(T &p_val, const std::vector<std::string> &p_defs);
    template<typename T> void ReadNextEnum(T &p_val, const std::vector<std::string> &p_defs);

    void Skip(int p_count = 1);
};

template<typename T> void LuaArgReader::ReadObject(T &p_val, const std::string &p_type)
{
    if(m_vm && !m_hasError)
    {
        if(m_currentArgument <= m_argumentsCount)
        {
            void *l_temp = nullptr;
            if(m_vm->ToObject(m_currentArgument, l_temp, p_type))
            {
                p_val = reinterpret_cast<T>(l_temp);
                m_currentArgument++;
            }
            else
                SetError("Expected " + p_type);
        }
        else
            SetError("Not enough arguments");
    }
}
template<typename T> void LuaArgReader::ReadObject(T &p_val)
{
    if(m_vm && !m_hasError)
    {
        if(m_currentArgument <= m_argumentsCount)
        {
            void *l_temp = nullptr;
            if(m_vm->ToObject(m_currentArgument, l_temp))
            {
                p_val = reinterpret_cast<T>(l_temp);
                m_currentArgument++;
            }
            else
                SetError("Expected object");
        }
        else
            SetError("Not enough arguments");
    }
}
template<typename T> void LuaArgReader::ReadNextObject(T &p_val, const std::string &p_type)
{
    if(m_vm && !m_hasError && (m_currentArgument <= m_argumentsCount))
    {
        void *l_temp = nullptr;
        if(m_vm->ToObject(m_currentArgument, l_temp, p_type))
            p_val = reinterpret_cast<T>(l_temp);
        m_currentArgument++;
    }
}
template<typename T> void LuaArgReader::ReadNextObject(T &p_val)
{
    if(m_vm && !m_hasError && (m_currentArgument <= m_argumentsCount))
    {
        void *l_temp = nullptr;
        if(m_vm->ToObject(m_currentArgument, l_temp))
            p_val = reinterpret_cast<T>(l_temp);
        m_currentArgument++;
    }
}

template<typename T> void LuaArgReader::ReadEnum(T &p_val, const std::vector<std::string> &p_defs)
{
    if(m_vm && !m_hasError)
    {
        if(m_currentArgument <= m_argumentsCount)
        {
            if(m_vm->IsString(m_currentArgument))
            {
                std::string l_val(m_vm->ToString(m_currentArgument));
                const auto l_iter = std::find(p_defs.begin(), p_defs.end(), l_val);
                if(l_iter != p_defs.end())
                {
                    p_val = static_cast<T>(std::distance(p_defs.begin(), l_iter));
                    m_currentArgument++;
                }
                else
                    SetError("Invalid enum string");
            }
            else
                SetError("Expected integer");
        }
        else
            SetError("Not enough arguments");
    }
}
template<typename T> void LuaArgReader::ReadNextEnum(T &p_val, const std::vector<std::string> &p_defs)
{
    if(m_vm && !m_hasError && (m_currentArgument <= m_argumentsCount))
    {
        if(m_vm->IsString(m_currentArgument))
        {
            std::string l_val(m_vm->ToString(m_currentArgument));
            const auto l_iter = std::find(p_defs.begin(), p_defs.end(), l_val);
            if(l_iter != p_defs.end())
                p_val = static_cast<T>(std::distance(p_defs.begin(), l_iter));
        }
        m_currentArgument++;
    }
}

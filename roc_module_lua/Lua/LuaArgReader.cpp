#include "stdafx.h"
#include "Lua/LuaArgReader.h"
#include "Lua/LuaVM.h"
#include "Logger.h"

LuaArgReader::LuaArgReader(lua_State *p_state)
{
    m_vm = LuaVM::GetVM(p_state);
    if(m_vm)
        m_argumentsCount = m_vm->GetTop();

    m_currentArgument = 1;
    m_hasError = false;
    m_returnCount = 0;
}

bool LuaArgReader::HasError() const
{
    return m_hasError;
}
void LuaArgReader::SetError(const std::string &p_error)
{
    m_hasError = true;
    m_error.assign(p_error);
}
void LuaArgReader::LogError() const
{
    if(m_vm && m_hasError)
    {
        std::string l_error("Warning: ");
        m_vm->GetStackInfo(l_error);
        l_error.append(": ");
        l_error.append(m_error);
        l_error.append(" at argument ");
        l_error.append(std::to_string(m_currentArgument));
        Logger::Log(l_error);
    }
}

int LuaArgReader::GetReturnValue() const
{
    return m_returnCount;
}

bool LuaArgReader::IsNextNil() const
{
    return (m_vm && !m_hasError && (m_currentArgument <= m_argumentsCount) && m_vm->IsNil(m_currentArgument));
}
void LuaArgReader::PushNil()
{
    if(m_vm)
    {
        m_vm->PushNil();
        m_returnCount++;
    }
}

bool LuaArgReader::IsNextBoolean() const
{
    return (m_vm && !m_hasError && (m_currentArgument <= m_argumentsCount) && m_vm->IsBoolean(m_currentArgument));
}
void LuaArgReader::ReadBoolean(bool &p_val)
{
    if(m_vm && !m_hasError)
    {
        if(m_currentArgument <= m_argumentsCount)
        {
            if(m_vm->IsBoolean(m_currentArgument))
            {
                p_val = m_vm->ToBoolean(m_currentArgument);
                m_currentArgument++;
            }
            else
                SetError("Expected boolean");
        }
        else
            SetError("Not enough arguments");
    }
}
void LuaArgReader::ReadNextBoolean(bool &p_val)
{
    if(m_vm && !m_hasError && (m_currentArgument <= m_argumentsCount))
    {
        if(m_vm->IsBoolean(m_currentArgument))
            p_val = m_vm->ToBoolean(m_currentArgument);
        m_currentArgument++;
    }
}
void LuaArgReader::PushBoolean(bool p_val)
{
    if(m_vm)
    {
        m_vm->PushBoolean(p_val);
        m_returnCount++;
    }
}

bool LuaArgReader::IsNextNumber() const
{
    return (m_vm && !m_hasError && (m_currentArgument <= m_argumentsCount) && m_vm->IsNumber(m_currentArgument));
}
void LuaArgReader::ReadNumber(double &p_val)
{
    if(m_vm && !m_hasError)
    {
        if(m_currentArgument <= m_argumentsCount)
        {
            if(m_vm->IsNumber(m_currentArgument))
            {
                p_val = m_vm->ToNumber(m_currentArgument);
                m_currentArgument++;
            }
            else
                SetError("Expected number");
        }
        else
            SetError("Not enough arguments");
    }
}
void LuaArgReader::ReadNumber(float &p_val)
{
    if(m_vm && !m_hasError)
    {
        if(m_currentArgument <= m_argumentsCount)
        {
            if(m_vm->IsNumber(m_currentArgument))
            {
                p_val = static_cast<float>(m_vm->ToNumber(m_currentArgument));
                m_currentArgument++;
            }
            else
                SetError("Expected number");
        }
        else
            SetError("Not enough arguments");
    }
}
void LuaArgReader::ReadNextNumber(double &p_val)
{
    if(m_vm && !m_hasError && (m_currentArgument <= m_argumentsCount))
    {
        if(m_vm->IsNumber(m_currentArgument))
            p_val = m_vm->ToNumber(m_currentArgument);
        m_currentArgument++;
    }
}
void LuaArgReader::ReadNextNumber(float &p_val)
{
    if(m_vm && !m_hasError && (m_currentArgument <= m_argumentsCount))
    {
        if(m_vm->IsNumber(m_currentArgument))
            p_val = static_cast<float>(m_vm->ToNumber(m_currentArgument));
        m_currentArgument++;
    }
}
void LuaArgReader::PushNumber(double p_val)
{
    if(m_vm)
    {
        m_vm->PushNumber(p_val);
        m_returnCount++;
    }
}
void LuaArgReader::PushNumber(float p_val)
{
    if(m_vm)
    {
        m_vm->PushNumber(p_val);
        m_returnCount++;
    }
}

bool LuaArgReader::IsNextInteger() const
{
    return (m_vm && !m_hasError && (m_currentArgument <= m_argumentsCount) && m_vm->IsInteger(m_currentArgument));
}
void LuaArgReader::ReadInteger(std::int64_t &p_val)
{
    if(m_vm && !m_hasError)
    {
        if(m_currentArgument <= m_argumentsCount)
        {
            if(m_vm->IsInteger(m_currentArgument))
            {
                p_val = m_vm->ToInteger(m_currentArgument);
                m_currentArgument++;
            }
            else
                SetError("Expected integer");
        }
        else
            SetError("Not enough arguments");
    }
}
void LuaArgReader::ReadInteger(size_t &p_val)
{
    if(m_vm && !m_hasError)
    {
        if(m_currentArgument <= m_argumentsCount)
        {
            if(m_vm->IsInteger(m_currentArgument))
            {
                p_val = m_vm->ToInteger(m_currentArgument);
                m_currentArgument++;
            }
            else
                SetError("Expected integer");
        }
        else
            SetError("Not enough arguments");
    }
}
void LuaArgReader::ReadInteger(int &p_val)
{
    if(m_vm && !m_hasError)
    {
        if(m_currentArgument <= m_argumentsCount)
        {
            if(m_vm->IsInteger(m_currentArgument))
            {
                p_val = static_cast<int>(m_vm->ToInteger(m_currentArgument));
                m_currentArgument++;
            }
            else
                SetError("Expected integer");
        }
        else
            SetError("Not enough arguments");
    }
}
void LuaArgReader::ReadNextInteger(std::int64_t &p_val)
{
    if(m_vm && !m_hasError && (m_currentArgument <= m_argumentsCount))
    {
        if(m_vm->IsInteger(m_currentArgument))
            p_val = m_vm->ToInteger(m_currentArgument);
        m_currentArgument++;
    }
}
void LuaArgReader::ReadNextInteger(size_t &p_val)
{
    if(m_vm && !m_hasError && (m_currentArgument <= m_argumentsCount))
    {
        if(m_vm->IsInteger(m_currentArgument))
            p_val = m_vm->ToInteger(m_currentArgument);
        m_currentArgument++;
    }
}
void LuaArgReader::ReadNextInteger(int &p_val)
{
    if(m_vm && !m_hasError && (m_currentArgument <= m_argumentsCount))
    {
        if(m_vm->IsInteger(m_currentArgument))
            p_val = static_cast<int>(m_vm->ToInteger(m_currentArgument));
        m_currentArgument++;
    }
}
void LuaArgReader::PushInteger(std::int64_t p_val)
{
    if(m_vm)
    {
        m_vm->PushInteger(p_val);
        m_returnCount++;
    }
}
void LuaArgReader::PushInteger(size_t p_val)
{
    if(m_vm)
    {
        m_vm->PushInteger(static_cast<std::int64_t>(p_val));
        m_returnCount++;
    }
}
void LuaArgReader::PushInteger(int p_val)
{
    if(m_vm)
    {
        m_vm->PushInteger(p_val);
        m_returnCount++;
    }
}

bool LuaArgReader::IsNextString() const
{
    return (m_vm && !m_hasError && (m_currentArgument <= m_argumentsCount) && m_vm->IsString(m_currentArgument));
}
void LuaArgReader::ReadString(std::string &p_val)
{
    if(m_vm && !m_hasError)
    {
        if(m_currentArgument <= m_argumentsCount)
        {
            if(m_vm->IsString(m_currentArgument))
            {
                p_val.assign(m_vm->ToString(m_currentArgument));
                m_currentArgument++;
            }
            else
                SetError("Expected integer");
        }
        else
            SetError("Not enough arguments");
    }
}
void LuaArgReader::ReadNextString(std::string &p_val)
{
    if(m_vm && !m_hasError && (m_currentArgument <= m_argumentsCount))
    {
        if(m_vm->IsString(m_currentArgument))
            p_val.assign(m_vm->ToString(m_currentArgument));
        m_currentArgument++;
    }
}
void LuaArgReader::PushString(const std::string &p_val)
{
    if(m_vm)
    {
        m_vm->PushString(p_val);
        m_returnCount++;
    }
}

bool LuaArgReader::IsNextObject() const
{
    return (m_vm && !m_hasError && (m_currentArgument <= m_argumentsCount) && m_vm->IsObject(m_currentArgument));
}
void LuaArgReader::PushObject(void* p_val, const std::string &p_type, bool p_external)
{
    if(m_vm)
    {
        m_vm->PushObject(p_val, p_type, p_external);
        m_returnCount++;
    }
}

void LuaArgReader::Skip(int p_count)
{
    if(m_vm && !m_hasError)
        m_currentArgument += p_count;
}

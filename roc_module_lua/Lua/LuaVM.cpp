#include "stdafx.h"
#include "Lua/LuaVM.h"
#include "Lua/LuaDefs.h"
#include "Logger.h"
#include "Utils.h"

const char* const g_objectsPool = "___ObjectsPool";
const char* const g_methods = "__methods";
const char* const g_propGet = "__propGet";
const char* const g_propSet = "__propSet";

LuaVM* LuaVM::ms_instance = nullptr;

LuaVM::LuaVM()
{
    if(!ms_instance)
        ms_instance = this;

    m_state = luaL_newstate();
    luaL_requiref(m_state, "_G", luaopen_base, 1);
    luaL_requiref(m_state, "coroutine", luaopen_coroutine, 1);
    luaL_requiref(m_state, "table", luaopen_table, 1);
    luaL_requiref(m_state, "string", luaopen_string, 1);
    luaL_requiref(m_state, "math", luaopen_math, 1);
    luaL_requiref(m_state, "utf8", luaopen_utf8, 1);

    // Table with weak values for objects
    lua_newtable(m_state);
    lua_newtable(m_state);
    lua_pushstring(m_state, "v");
    lua_setfield(m_state, -2, "__mode");
    lua_pushcfunction(m_state, NilResultFunction);
    lua_setfield(m_state, -2, "__metatable");
    lua_setmetatable(m_state, -2); // Combines two previous tables
    lua_setfield(m_state, LUA_REGISTRYINDEX, g_objectsPool);
}
LuaVM::~LuaVM()
{
    lua_close(m_state);

    if(ms_instance == this)
        ms_instance = nullptr;
}

LuaVM* LuaVM::GetVM(lua_State *p_state)
{
    if(ms_instance && (ms_instance->m_state == p_state))
        return ms_instance;
    return nullptr;
}

// GC
void LuaVM::PerformGC()
{
    lua_gc(m_state, LUA_GCSTEP, 0);
}

// Generic
int LuaVM::GetTop() const
{
    return lua_gettop(m_state);
}

bool LuaVM::IsBoolean(int p_index) const
{
    return lua_isboolean(m_state, p_index);
}
bool LuaVM::ToBoolean(int p_index)
{
    return (lua_toboolean(m_state, p_index) == 1);
}
void LuaVM::PushBoolean(bool p_val)
{
    lua_pushboolean(m_state, p_val ? 1 : 0);
}

bool LuaVM::IsInteger(int p_index) const
{
    return (lua_isinteger(m_state, p_index) == 1);
}
std::int64_t LuaVM::ToInteger(int p_index)
{
    return lua_tointeger(m_state, p_index);
}
void LuaVM::PushInteger(std::int64_t p_val)
{
    lua_pushinteger(m_state, p_val);
}

bool LuaVM::IsNumber(int p_index) const
{
    return (lua_isnumber(m_state, p_index) == 1);
}
double LuaVM::ToNumber(int p_index)
{
    return lua_tonumber(m_state, p_index);
}
void LuaVM::PushNumber(double p_val)
{
    lua_pushnumber(m_state, p_val);
}

bool LuaVM::IsString(int p_index) const
{
    return (lua_isstring(m_state, p_index) == 1);
}
const char* LuaVM::ToString(int p_index)
{
    return lua_tostring(m_state, p_index);
}
void LuaVM::PushString(const std::string &p_str)
{
    lua_pushstring(m_state, p_str.c_str());
}

bool LuaVM::IsUserdata(int p_index) const
{
    return (lua_isuserdata(m_state, p_index) == 1);
}
void* LuaVM::ToUserdata(int p_index)
{
    return lua_touserdata(m_state, p_index);
}

bool LuaVM::IsNil(int p_index) const
{
    return lua_isnil(m_state, p_index);
}
void LuaVM::PushNil()
{
    lua_pushnil(m_state);
}

bool LuaVM::IsFunction(int p_index) const
{
    return lua_isfunction(m_state, p_index);
}
void LuaVM::PushFunction(lua_CFunction p_func)
{
    lua_pushcfunction(m_state, p_func);
}

// Objects
bool LuaVM::IsObject(int p_index) const
{
    return IsUserdata(p_index);
}
bool LuaVM::ToObject(int p_index, void* &p_obj, const std::string &p_type)
{
    bool l_result = false;
    if(IsUserdata(p_index))
    {
        std::int64_t l_hash = 0;
        std::memcpy(&l_hash, ToUserdata(p_index), sizeof(void*));
        if(l_hash != 0)
        {
            const auto &l_iter = m_objectsPool.find(l_hash);
            if((l_iter != m_objectsPool.end()) && (l_iter->second->m_type == p_type))
            {
                p_obj = l_iter->second->m_object;
                l_result = true;
            }
        }
    }
    return l_result;
}
bool LuaVM::ToObject(int p_index, void* &p_obj)
{
    bool l_result = false;
    if(IsUserdata(p_index))
    {
        std::int64_t l_hash = 0;
        std::memcpy(&l_hash, ToUserdata(p_index), sizeof(void*));
        if(l_hash != 0)
        {
            const auto &l_iter = m_objectsPool.find(l_hash);
            if(l_iter != m_objectsPool.end())
            {
                p_obj = l_iter->second->m_object;
                l_result = true;
            }
        }
    }
    return l_result;
}
void LuaVM::PushObject(void* p_obj, const std::string &p_type, bool p_external)
{
    auto l_hash = reinterpret_cast<std::int64_t>(p_obj);
    const auto &l_iter = m_objectsPool.find(l_hash);
    if(l_iter != m_objectsPool.end())
        l_iter->second->m_references++;
    else
        m_objectsPool.insert(std::make_pair(l_hash, new LuaObject(p_obj, p_type, p_external)));

    lua_getfield(m_state, LUA_REGISTRYINDEX, g_objectsPool);
    if(lua_geti(m_state, -1, l_hash) == LUA_TNIL)
    {
        lua_pop(m_state, 1);
        std::memcpy(lua_newuserdata(m_state, sizeof(void*)), &l_hash, sizeof(void*));
        luaL_setmetatable(m_state, p_type.c_str());
        lua_pushvalue(m_state, -1);
        lua_seti(m_state, -3, l_hash);
    }
    lua_remove(m_state, -2);
}

// Functions
bool LuaVM::IsFunctionPresent(const std::string &p_name) const
{
    bool l_result = (lua_getglobal(m_state, p_name.c_str()) == LUA_TFUNCTION);
    lua_pop(m_state, 1);
    return l_result;
}

int LuaVM::GetFunctionReference(const std::string &p_name)
{
    int l_result = 0;

    if(lua_getglobal(m_state, p_name.c_str()) == LUA_TFUNCTION)
        l_result = luaL_ref(m_state, LUA_REGISTRYINDEX);
    else
        lua_pop(m_state, 1);

    return l_result;
}

void LuaVM::RegisterFunction(const std::string &p_name, lua_CFunction p_func)
{
    lua_pushcfunction(m_state, p_func);
    lua_setglobal(m_state, p_name.c_str());
}

void LuaVM::CallFunction(int p_ref, const ROC::ICustomArguments *p_args)
{
    if(lua_rawgeti(m_state, LUA_REGISTRYINDEX, p_ref) == LUA_TFUNCTION)
    {
        if(p_args)
        {
            for(size_t i = 0U, j = p_args->GetArgumentsCount(); i < j; i++)
                PushArgument(p_args->GetArgument(i));
        }
        if(lua_pcall(m_state, p_args ? static_cast<int>(p_args->GetArgumentsCount()) : 0, 0, 0) != 0)
        {
            std::string l_error(lua_tostring(m_state, -1));
            Logger::Log(l_error);
            lua_pop(m_state, 1);
        }
    }
    else
        lua_pop(m_state, 1);
}

// Classes
void LuaVM::RegisterLuaClass(
    const std::string &p_type,
    lua_CFunction p_ctor,
    const std::vector<LuaPropDef> *p_staticProps,
    const std::vector<LuaMethodDef> *p_staticMethods,
    const std::vector<LuaMethodDef> *p_metaMethods,
    const std::vector<LuaPropDef> *p_instanceProps,
    const std::vector<LuaMethodDef> *p_instanceMethods
)
{
    // Static definition
    lua_newtable(m_state); // {}
    lua_newtable(m_state); // {}

    if(p_ctor)
    {
        lua_pushcfunction(m_state, p_ctor);
        lua_setfield(m_state, -2, "__call");
    }

    lua_pushcfunction(m_state, NilResultFunction);
    lua_setfield(m_state, -2, "__metatable");

    lua_pushcfunction(m_state, ClassStaticGet);
    lua_setfield(m_state, -2, "__index");

    lua_pushcfunction(m_state, ClassStaticSet);
    lua_setfield(m_state, -2, "__newindex");

    lua_newtable(m_state);

    if(p_staticProps)
    {
        for(const auto& l_prop : *p_staticProps)
        {
            if(l_prop.m_get) // property get
            {
                lua_pushcfunction(m_state, l_prop.m_get);
                lua_setfield(m_state, -2, l_prop.m_name.c_str());
            }
        }
    }
    lua_setfield(m_state, -2, g_propGet);

    lua_newtable(m_state); // {}
    if(p_staticProps)
    {
        for(const auto& l_prop : *p_staticProps)
        {
            if(l_prop.m_set) // Property set
            {
                lua_pushcfunction(m_state, l_prop.m_set);
                lua_setfield(m_state, -2, l_prop.m_name.c_str());
            }
        }
    }
    lua_setfield(m_state, -2, g_propSet);

    lua_newtable(m_state);
    if(p_staticMethods)
    {
        for(const auto& l_method : *p_staticMethods)
        {
            lua_pushcfunction(m_state, l_method.m_func);
            lua_setfield(m_state, -2, l_method.m_name.c_str());
        }
    }
    lua_setfield(m_state, -2, g_methods);

    lua_setmetatable(m_state, -2); // Combines two previous tables
    lua_setglobal(m_state, p_type.c_str()); // Sets as global

    // Instance definition
    luaL_newmetatable(m_state, p_type.c_str()); // Registry metatable

    lua_pushcfunction(m_state, NilResultFunction);
    lua_setfield(m_state, -2, "__metatable");

    lua_pushcfunction(m_state, ClassInstanceGet);
    lua_setfield(m_state, -2, "__index");

    lua_pushcfunction(m_state, ClassInstanceSet);
    lua_setfield(m_state, -2, "__newindex");

    lua_pushcfunction(m_state, ObjectsGC);
    lua_setfield(m_state, -2, "__gc"); // Garbage collector for referenced objects as userdata

    if(p_metaMethods)
    {
        for(const auto &l_meta : *p_metaMethods)
        {
            lua_pushcfunction(m_state, l_meta.m_func);
            lua_setfield(m_state, -2, l_meta.m_name.c_str());
        }
    }

    lua_newtable(m_state);
    if(p_instanceProps)
    {
        for(const auto& l_prop : *p_instanceProps)
        {
            if(l_prop.m_get)
            {
                lua_pushcfunction(m_state, l_prop.m_get);
                lua_setfield(m_state, -2, l_prop.m_name.c_str());
            }
        }
    }
    lua_setfield(m_state, -2, g_propGet);

    lua_newtable(m_state);
    if(p_instanceProps)
    {
        for(const auto& l_prop : *p_instanceProps)
        {
            if(l_prop.m_set) // Property set
            {
                lua_pushcfunction(m_state, l_prop.m_set);
                lua_setfield(m_state, -2, l_prop.m_name.c_str());
            }
        }
    }
    lua_setfield(m_state, -2, g_propSet);

    lua_newtable(m_state);
    if(p_instanceMethods)
    {
        for(const auto& l_method : *p_instanceMethods)
        {
            lua_pushcfunction(m_state, l_method.m_func);
            lua_setfield(m_state, -2, l_method.m_name.c_str());
        }
    }
    lua_setfield(m_state, -2, g_methods);

    lua_pop(m_state, 1); // Pop metatable
}

int LuaVM::ClassStaticGet(lua_State *p_state)
{
    // Current stack - 1-table, 2-key
    if(!lua_isstring(p_state, 2)) // Not a string as key
    {
        lua_pushnil(p_state);
        return 1;
    }
    const char *l_key = lua_tostring(p_state, 2);

    luaL_getmetafield(p_state, 1, g_methods); // table on top
    if(lua_getfield(p_state, -1, l_key) == LUA_TFUNCTION)
    {
        // function is on top
        lua_remove(p_state, -2); // remove table and shift stack down
        return 1;
    }

    // Not a method, maybe a prop?
    lua_pop(p_state, 2); // remove undesired value and table from stack
    luaL_getmetafield(p_state, 1, g_propGet); // table is on top
    if(lua_getfield(p_state, -1, l_key) == LUA_TFUNCTION)
    {
        lua_call(p_state, 0, 1); // result is on top
        lua_remove(p_state, -2); // remove table and shift stack down
        return 1;
    }

    // Nothing found, push nil
    lua_pop(p_state, 2); // remove undesired value and table from stack
    lua_pushnil(p_state);
    return 1;
}

int LuaVM::ClassStaticSet(lua_State *p_state)
{
    // Current stack - 1-table, 2-key, 3-value
    if(!lua_isstring(p_state, 2)) // Not a string as key
        return 0;
    const char *l_key = lua_tostring(p_state, 2);

    luaL_getmetafield(p_state, 1, g_propSet); // table on top
    if(lua_getfield(p_state, -1, l_key) == LUA_TFUNCTION)
    {
        // now function is on top
        lua_pushvalue(p_state, 3); // copy value
        lua_call(p_state, 1, 0); // call, no result should return
        lua_pop(p_state, 1); // remove table from stack
        return 0;
    }

    // Nothing found
    lua_pop(p_state, 2); // remove table and value from stack
    return 0;
}

int LuaVM::ClassInstanceGet(lua_State *p_state)
{
    // Current stack - 1-userdata, 2-key
    if(!lua_isstring(p_state, 2)) // Not a string as key
    {
        lua_pushnil(p_state);
        return 1;
    }
    const char *l_key = lua_tostring(p_state, 2);

    luaL_getmetafield(p_state, 1, g_methods); // table is on top
    if(lua_getfield(p_state, -1, l_key) == LUA_TFUNCTION)
    {
        // result function on top
        lua_remove(p_state, -2); // remove table and shift stack down
        return 1;
    }

    // Not a method, maybe a prop?
    lua_pop(p_state, 2); // remove undesired value and table from stack
    luaL_getmetafield(p_state, 1, g_propGet); // table is on top
    if(lua_getfield(p_state, -1, l_key) == LUA_TFUNCTION)
    {
        // function is on top
        lua_pushvalue(p_state, 1); // copy userdata on top
        lua_call(p_state, 1, 1); // result on top
        lua_remove(p_state, -2); // remove table and shift stack down
        return 1;
    }

    // Nothing found, push nil
    lua_pop(p_state, 2); // remove undesired value and table from stack
    lua_pushnil(p_state);
    return 1;
}

int LuaVM::ClassInstanceSet(lua_State *p_state)
{
    // Current stack - 1-userdata, 2-key, 3-value
    if(!lua_isstring(p_state, 2)) // Not a string as key
        return 0;
    const char* l_key = lua_tostring(p_state, 2);

    luaL_getmetafield(p_state, 1, g_propSet); // table on top
    if(lua_getfield(p_state, -1, l_key) == LUA_TFUNCTION)
    {
        // function is on top
        lua_pushvalue(p_state, 1); // copy userdata on top
        lua_pushvalue(p_state, 3); // copy value on top
        lua_call(p_state, 2, 0); // call, no result should return
        lua_pop(p_state, 1); // remove table from stack
        return 0;
    }

    // Nothing found
    lua_pop(p_state, 2); // remove table and value from stack
    return 0;
}

int LuaVM::NilResultFunction(lua_State *p_state)
{
    lua_pushnil(p_state);
    return 1;
}

int LuaVM::ObjectsGC(lua_State *p_state)
{
    if(ms_instance && ms_instance->IsUserdata(1))
    {
        std::int64_t l_hash = 0;
        std::memcpy(&l_hash, ms_instance->ToUserdata(1), sizeof(void*));
        if(l_hash != 0)
        {
            const auto& l_iter = ms_instance->m_objectsPool.find(l_hash);
            if(l_iter != ms_instance->m_objectsPool.end())
            {
                LuaObject *l_obj = l_iter->second;
                l_obj->m_references--;
                if(l_obj->m_references == 0)
                {
                    if(!l_obj->m_external)
                        Utils::DeleteByType(l_obj->m_object, l_obj->m_type);

                    delete l_obj;
                    ms_instance->m_objectsPool.erase(l_iter);
                }
            }
        }
    }
    return 0;
}

// Extended push
void LuaVM::PushArgument(const ROC::ICustomArgument *p_arg)
{
    switch(p_arg->GetType())
    {
        case ROC::ICustomArgument::CAT_Boolean:
            PushBoolean(p_arg->GetBoolean());
            break;

        case ROC::ICustomArgument::CAT_Integer:
        case ROC::ICustomArgument::CAT_UInteger:
            PushInteger(p_arg->GetInteger());
            break;

        case ROC::ICustomArgument::CAT_Float:
        case ROC::ICustomArgument::CAT_Double:
            PushInteger(p_arg->GetInteger());
            break;

        case ROC::ICustomArgument::CAT_String:
            PushString(p_arg->GetString());
            break;

        case ROC::ICustomArgument::CAT_Element:
            PushObject(p_arg->GetPointer(), p_arg->GetElement()->GetElementTypeName(), true);
            break;

        default:
            PushNil();
    }
}

// Script load
void LuaVM::LoadScript(const std::string &p_path)
{
    if(luaL_loadfile(m_state, p_path.c_str()) || lua_pcall(m_state, 0, 0, NULL))
    {
        std::string l_error(lua_tostring(m_state, -1));
        Logger::Log(l_error);
        lua_pop(m_state, 1);
    }
}

// Debug
void LuaVM::GetStackInfo(std::string &p_str)
{
    for(int i = 0;; i++)
    {
        lua_Debug l_debug;
        if(!lua_getstack(m_state, i, &l_debug))
        {
            p_str.append("[]:");
            break;
        }

        lua_getinfo(m_state, "nSl", &l_debug);
        if(l_debug.currentline < 0)
            continue;

        p_str.push_back('[');
        p_str.append(l_debug.source);
        p_str.append("]:");
        p_str.append(std::to_string(l_debug.currentline));
        break;
    }
}

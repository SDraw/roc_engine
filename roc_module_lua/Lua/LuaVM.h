#pragma once

struct LuaPropDef;
struct LuaMethodDef;
class LuaVM final
{
    struct LuaObject
    {
    public:
        void* m_object;
        bool m_external;
        std::string m_type;

        LuaObject(void *p_obj, const std::string &p_type, bool p_external)
        {
            m_object = p_obj;
            m_external = p_external;
            m_type.assign(p_type);
        }
    };

    static LuaVM *ms_instance;

    lua_State *m_state;

    std::unordered_map<std::int64_t, LuaObject*> m_objectsPool;

    LuaVM(const LuaVM &that) = delete;
    LuaVM& operator=(const LuaVM &that) = delete;

    void PushArgument(const ROC::ICustomArgument *p_arg);

    static int ClassStaticGet(lua_State *p_state);
    static int ClassStaticSet(lua_State *p_state);
    static int ClassInstanceGet(lua_State *p_state);
    static int ClassInstanceSet(lua_State *p_state);
    static int NilResultFunction(lua_State *p_state);
    static int ObjectsGC(lua_State *p_state);
public:
    LuaVM();
    ~LuaVM();

    static LuaVM* GetVM(lua_State *p_state);

    void PerformGC();

    int GetTop() const;

    bool IsBoolean(int p_index) const;
    bool ToBoolean(int p_index);
    void PushBoolean(bool p_val);

    bool IsInteger(int p_index) const;
    std::int64_t ToInteger(int p_index);
    void PushInteger(std::int64_t p_val);

    bool IsNumber(int p_index) const;
    double ToNumber(int p_index);
    void PushNumber(double p_val);

    bool IsString(int p_index) const;
    const char* ToString(int p_index);
    void PushString(const std::string &p_str);

    bool IsUserdata(int p_index) const;
    void* ToUserdata(int p_index);

    bool IsNil(int p_index) const;
    void PushNil();

    bool IsFunction(int p_index) const;
    void PushFunction(lua_CFunction p_func);

    bool IsObject(int p_index) const;
    void PushObject(void* p_obj, const std::string &p_type, bool p_external);
    bool ToObject(int p_index, void* &p_obj, const std::string &p_type);
    bool ToObject(int p_index, void* &p_obj);

    bool IsFunctionPresent(const std::string &p_name) const;
    int GetFunctionReference(const std::string &p_name);
    void RegisterFunction(const std::string &p_name, lua_CFunction p_func);
    void CallFunction(int p_ref, const ROC::ICustomArguments *p_args);

    void RegisterLuaClass(
        const std::string &p_type,
        lua_CFunction p_ctor,
        const std::vector<LuaPropDef> *p_staticProps,
        const std::vector<LuaMethodDef> *p_staticMethods,
        const std::vector<LuaMethodDef> *p_metaMethods,
        const std::vector<LuaPropDef> *p_instanceProps,
        const std::vector<LuaMethodDef> *p_instanceMethods
    );

    void LoadScript(const std::string &p_path);

    void GetStackInfo(std::string &p_str);
};


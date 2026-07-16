using System;
using System.Collections.Generic;

namespace LuaSharp.Lua
{
    public sealed partial class LuaVM : IDisposable
    {
        public struct LuaClassDefinition
        {
            public string m_name;
            public LuaInterop.lua_CFunction m_constructor;
            public List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)> m_staticProperties; // Name-Get-Set
            public List<(string, LuaInterop.lua_CFunction)> m_staticMethods; // Name-Method
            public List<(string, LuaInterop.lua_CFunction)> m_metaMethods; // Name-Method
            public List<(string, LuaInterop.lua_CFunction, LuaInterop.lua_CFunction)> m_instanceProperties; // Name-Get-Set
            public List<(string, LuaInterop.lua_CFunction)> m_instanceMethods; // Name-Method
        }

        const string c_propGet = "__propGet";
        const string c_propSet = "__propSet";
        const string c_methods = "__methods";

        // CSharp's GC is stinky
        static readonly LuaInterop.lua_CFunction ms_classStaticGetDelegate = ClassStaticGet;
        static readonly LuaInterop.lua_CFunction ms_classStaticSetDelegate = ClassStaticSet;
        static readonly LuaInterop.lua_CFunction ms_instanceGetDelegate = ClassInstanceGet;
        static readonly LuaInterop.lua_CFunction ms_instanceSetDelegate = ClassInstanceSet;

        // Classes registration and handling
        public void RegisterClass(LuaClassDefinition p_definition)
        {
            if(p_definition.m_name == null)
                return;

            // Static definition
            LuaInterop.lua_newtable(m_state); // {}
            LuaInterop.lua_newtable(m_state); // {}

            LuaInterop.lua_pushcfunction(m_state, p_definition.m_constructor ?? ms_nilFunctionDelegate);
            LuaInterop.lua_setfield(m_state, -2, "__call");

            LuaInterop.lua_pushcfunction(m_state, ms_nilFunctionDelegate);
            LuaInterop.lua_setfield(m_state, -2, "__metatable");

            LuaInterop.lua_pushcfunction(m_state, ms_classStaticGetDelegate);
            LuaInterop.lua_setfield(m_state, -2, "__index");

            LuaInterop.lua_pushcfunction(m_state, ms_classStaticSetDelegate);
            LuaInterop.lua_setfield(m_state, -2, "__newindex");

            // Static properties
            LuaInterop.lua_newtable(m_state);
            if(p_definition.m_staticProperties != null)
            {
                foreach(var l_prop in p_definition.m_staticProperties)
                {
                    if(l_prop.Item2 != null)
                    {
                        LuaInterop.lua_pushcfunction(m_state, l_prop.Item2);
                        LuaInterop.lua_setfield(m_state, -2, l_prop.Item1);
                    }
                }
            }
            LuaInterop.lua_setfield(m_state, -2, c_propGet);

            LuaInterop.lua_newtable(m_state); // {}
            if(p_definition.m_staticProperties != null)
            {
                foreach(var l_prop in p_definition.m_staticProperties)
                {
                    if(l_prop.Item3 != null) // Property set
                    {
                        LuaInterop.lua_pushcfunction(m_state, l_prop.Item3);
                        LuaInterop.lua_setfield(m_state, -2, l_prop.Item1);
                    }
                }
            }
            LuaInterop.lua_setfield(m_state, -2, c_propSet);

            // Static methods
            LuaInterop.lua_newtable(m_state);
            if(p_definition.m_staticMethods != null)
            {
                foreach(var l_pair in p_definition.m_staticMethods)
                {
                    LuaInterop.lua_pushcfunction(m_state, l_pair.Item2);
                    LuaInterop.lua_setfield(m_state, -2, l_pair.Item1);
                }
            }
            LuaInterop.lua_setfield(m_state, -2, c_methods);

            LuaInterop.lua_setmetatable(m_state, -2); // Combines two previous tables
            LuaInterop.lua_setglobal(m_state, p_definition.m_name); // Sets as global

            // Instance definition
            LuaInterop.luaL_newmetatable(m_state, p_definition.m_name); // Registry metatable

            LuaInterop.lua_pushcfunction(m_state, ms_nilFunctionDelegate);
            LuaInterop.lua_setfield(m_state, -2, "__metatable");

            LuaInterop.lua_pushcfunction(m_state, ms_instanceGetDelegate);
            LuaInterop.lua_setfield(m_state, -2, "__index");

            LuaInterop.lua_pushcfunction(m_state, ms_instanceSetDelegate);
            LuaInterop.lua_setfield(m_state, -2, "__newindex");

            LuaInterop.lua_pushcfunction(m_state, ms_objectsGCDelegate);
            LuaInterop.lua_setfield(m_state, -2, "__gc"); // Garbage collector for referenced objects as userdata

            // Metamethods
            if(p_definition.m_metaMethods != null)
            {
                foreach(var l_pair in p_definition.m_metaMethods)
                {
                    LuaInterop.lua_pushcfunction(m_state, l_pair.Item2);
                    LuaInterop.lua_setfield(m_state, -2, l_pair.Item1); // Push additional metatable methods if any
                }
            }

            // Properties
            LuaInterop.lua_newtable(m_state);
            if(p_definition.m_instanceProperties != null)
            {
                foreach(var l_prop in p_definition.m_instanceProperties)
                {
                    if(l_prop.Item2 != null)
                    {
                        LuaInterop.lua_pushcfunction(m_state, l_prop.Item2);
                        LuaInterop.lua_setfield(m_state, -2, l_prop.Item1);
                    }
                }
            }
            LuaInterop.lua_setfield(m_state, -2, c_propGet);

            LuaInterop.lua_newtable(m_state);
            if(p_definition.m_instanceProperties != null)
            {
                foreach(var l_prop in p_definition.m_instanceProperties)
                {
                    if(l_prop.Item3 != null) // Property set
                    {
                        LuaInterop.lua_pushcfunction(m_state, l_prop.Item3);
                        LuaInterop.lua_setfield(m_state, -2, l_prop.Item1);
                    }
                }
            }
            LuaInterop.lua_setfield(m_state, -2, c_propSet);

            // Instance methods
            LuaInterop.lua_newtable(m_state);
            if(p_definition.m_instanceMethods != null)
            {
                foreach(var l_pair in p_definition.m_instanceMethods)
                {
                    LuaInterop.lua_pushcfunction(m_state, l_pair.Item2);
                    LuaInterop.lua_setfield(m_state, -2, l_pair.Item1);
                }
            }
            LuaInterop.lua_setfield(m_state, -2, c_methods);

            LuaInterop.lua_pop(m_state, 1); // Pop metatable
        }

        static int ClassStaticGet(IntPtr p_state)
        {
            // Current stack - 1-table, 2-key
            if(!LuaInterop.lua_isstring(p_state, 2)) // Not a string as key
            {
                LuaInterop.lua_pushnil(p_state);
                return 1;
            }
            string l_key = LuaInterop.lua_tostring(p_state, 2);

            LuaInterop.luaL_getmetafield(p_state, 1, c_methods); // table on top
            if(LuaInterop.lua_getfield(p_state, -1, l_key) == LuaInterop.LUA_TFUNCTION)
            {
                // function is on top
                LuaInterop.lua_remove(p_state, -2); // remove table and shift stack down
                return 1;
            }

            // Not a method, maybe a prop?
            LuaInterop.lua_pop(p_state, 2); // remove undesired value and table from stack
            LuaInterop.luaL_getmetafield(p_state, 1, c_propGet); // table is on top
            if(LuaInterop.lua_getfield(p_state, -1, l_key) == LuaInterop.LUA_TFUNCTION)
            {
                LuaInterop.lua_call(p_state, 0, 1); // result is on top
                LuaInterop.lua_remove(p_state, -2); // remove table and shift stack down
                return 1;
            }

            // Nothing found, push nil
            LuaInterop.lua_pop(p_state, 2); // remove undesired value and table from stack
            LuaInterop.lua_pushnil(p_state);
            return 1;
        }

        static int ClassStaticSet(IntPtr p_state)
        {
            // Current stack - 1-table, 2-key, 3-value
            if(!LuaInterop.lua_isstring(p_state, 2)) // Not a string as key
                return 0;
            string l_key = LuaInterop.lua_tostring(p_state, 2);

            LuaInterop.luaL_getmetafield(p_state, 1, c_propSet); // table on top
            if(LuaInterop.lua_getfield(p_state, -1, l_key) == LuaInterop.LUA_TFUNCTION)
            {
                // now function is on top
                LuaInterop.lua_pushvalue(p_state, 3); // copy value
                LuaInterop.lua_call(p_state, 1, 0); // call, no result should return
                LuaInterop.lua_pop(p_state, 1); // remove table from stack
                return 0;
            }

            // Nothing found
            LuaInterop.lua_pop(p_state, 2); // remove table and value from stack
            return 0;
        }

        static int ClassInstanceGet(IntPtr p_state)
        {
            // Current stack - 1-userdata, 2-key
            if(!LuaInterop.lua_isstring(p_state, 2)) // Not a string as key
            {
                LuaInterop.lua_pushnil(p_state);
                return 1;
            }
            string l_key = LuaInterop.lua_tostring(p_state, 2);

            LuaInterop.luaL_getmetafield(p_state, 1, c_methods); // table is on top
            if(LuaInterop.lua_getfield(p_state, -1, l_key) == LuaInterop.LUA_TFUNCTION)
            {
                // result function on top
                LuaInterop.lua_remove(p_state, -2); // remove table and shift stack down
                return 1;
            }

            // Not a method, maybe a prop?
            LuaInterop.lua_pop(p_state, 2); // remove undesired value and table from stack
            LuaInterop.luaL_getmetafield(p_state, 1, c_propGet); // table is on top
            if(LuaInterop.lua_getfield(p_state, -1, l_key) == LuaInterop.LUA_TFUNCTION)
            {
                // function is on top
                LuaInterop.lua_pushvalue(p_state, 1); // copy userdata on top
                LuaInterop.lua_call(p_state, 1, 1); // result on top
                LuaInterop.lua_remove(p_state, -2); // remove table and shift stack down
                return 1;
            }

            // Nothing found, push nil
            LuaInterop.lua_pop(p_state, 2); // remove undesired value and table from stack
            LuaInterop.lua_pushnil(p_state);
            return 1;
        }

        static int ClassInstanceSet(IntPtr p_state)
        {
            // Current stack - 1-userdata, 2-key, 3-value
            if(!LuaInterop.lua_isstring(p_state, 2)) // Not a string as key
                return 0;
            string l_key = LuaInterop.lua_tostring(p_state, 2);

            LuaInterop.luaL_getmetafield(p_state, 1, c_propSet); // table on top
            if(LuaInterop.lua_getfield(p_state, -1, l_key) == LuaInterop.LUA_TFUNCTION)
            {
                // function is on top
                LuaInterop.lua_pushvalue(p_state, 1); // copy userdata on top
                LuaInterop.lua_pushvalue(p_state, 3); // copy value on top
                LuaInterop.lua_call(p_state, 2, 0); // call, no result should return
                LuaInterop.lua_pop(p_state, 1); // remove table from stack
                return 0;
            }

            // Nothing found
            LuaInterop.lua_pop(p_state, 2); // remove table and value from stack
            return 0;
        }
    }
}

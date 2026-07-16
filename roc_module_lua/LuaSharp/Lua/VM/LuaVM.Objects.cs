using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace LuaSharp.Lua
{
    public sealed partial class LuaVM : IDisposable
    {
        const string c_objectsPool = "___ObjectsPool";

        readonly Dictionary<long, object> m_objectsMap = null;

        // CSharp's GC is stinky
        static readonly LuaInterop.lua_CFunction ms_objectsGCDelegate = ObjectsGC;

        // Objects push/get
        public void PushObject(object p_obj) => PushObject(p_obj, p_obj.GetType());
        public void PushObject(object p_obj, Type p_type)
        {
            long l_hash = Extensions.CombineInts(RuntimeHelpers.GetHashCode(p_obj), p_type.GetHashCode()); // Help ...
            if(!m_objectsMap.ContainsKey(l_hash))
                m_objectsMap.Add(l_hash, p_obj);

            LuaInterop.lua_getfield(m_state, LuaInterop.LUA_REGISTRYINDEX, c_objectsPool);
            if(LuaInterop.lua_geti(m_state, -1, l_hash) == LuaInterop.LUA_TNIL)
            {
                LuaInterop.lua_pop(m_state, 1);
                LuaInterop.lua_newuserdata(m_state, sizeof(long)).SetInt(l_hash);
                LuaInterop.luaL_setmetatable(m_state, p_type.Name);
                LuaInterop.lua_pushvalue(m_state, -1);
                LuaInterop.lua_seti(m_state, -3, l_hash);
            }
            LuaInterop.lua_remove(m_state, -2);
        }

        public bool GetObject<T>(int p_index, out T p_obj) where T : class
        {
            p_obj = null;
            bool l_result = false;
            if(IsUserdata(p_index))
            {
                long l_hash = GetUserdata(p_index).GetInt();
                if((l_hash != 0) && m_objectsMap.TryGetValue(l_hash, out var l_refObj) && (l_refObj is T))
                {
                    p_obj = l_refObj as T;
                    l_result = true;
                }
            }
            return l_result;
        }

        public bool IsObject(int p_index) => IsUserdata(p_index);

        static int ObjectsGC(IntPtr p_state)
        {
            LuaVM l_vm = GetVM(p_state);
            if((l_vm != null) && l_vm.IsUserdata(1))
            {
                long l_hash = l_vm.GetUserdata(1).GetInt();
                if((l_hash != 0) && l_vm.m_objectsMap.TryGetValue(l_hash, out var l_orc))
                    l_vm.m_objectsMap.Remove(l_hash);
            }
            return 0;
        }
    }
}

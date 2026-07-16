using System;
using System.Runtime.InteropServices;

namespace LuaSharp.Lua
{
    public static class LuaInterop
    {
        const string ms_binaryName = "lua54.dll";

        public const int LUAI_MAXSTACK = 1000000; // for x64
        public const int LUA_REGISTRYINDEX = (-LUAI_MAXSTACK - 1000);
        public const int LUA_MULTRET = -1;

        public const int LUA_OK = 0;
        public const int LUA_YIELD = 1;
        public const int LUA_ERRRUN = 2;
        public const int LUA_ERRSYNTAX = 3;
        public const int LUA_ERRMEM = 4;
        public const int LUA_ERRERR = 5;

        public const int LUA_TNIL = 0;
        public const int LUA_TBOOLEAN = 1;
        public const int LUA_TLIGHTUSERDATA = 2;
        public const int LUA_TNUMBER = 3;
        public const int LUA_TSTRING = 4;
        public const int LUA_TTABLE = 5;
        public const int LUA_TFUNCTION = 6;
        public const int LUA_TUSERDATA = 7;
        public const int LUA_TTHREAD = 8;

        public const int LUA_GCSTOP = 0;
        public const int LUA_GCRESTART = 1;
        public const int LUA_GCCOLLECT = 2;
        public const int LUA_GCCOUNT = 3;
        public const int LUA_GCCOUNTB = 4;
        public const int LUA_GCSTEP = 5;
        public const int LUA_GCSETPAUSE = 6;
        public const int LUA_GCSETSTEPMUL = 7;
        public const int LUA_GCISRUNNING = 9;

        public const int LUA_IDSIZE = 60;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int lua_CFunction(IntPtr L);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int lua_KFunction(IntPtr L, int status, int ctx);

        public struct lua_Debug
        {
            public int event_;
            public IntPtr name;
            public IntPtr namewhat;
            public IntPtr what;
            public IntPtr source;
            public long srclen;
            public int currentline;
            public int linedefined;
            public int lastlinedefined;
            public char nups;
            public char nparams;
            public char isvararg;
            public char istailcall;
            public short ftransfer;
            public short ntransfer;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = LUA_IDSIZE)]
            public char[] short_src;
            public IntPtr i_ci;
        }

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr luaL_newstate();

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void lua_close(IntPtr L);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void luaL_requiref(IntPtr L, string modname, lua_CFunction openf, int glb);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int luaopen_base(IntPtr L);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int luaopen_coroutine(IntPtr L);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int luaopen_table(IntPtr L);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int luaopen_string(IntPtr L);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int luaopen_math(IntPtr L);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int luaopen_utf8(IntPtr L);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void luaL_newmetatable(IntPtr L, string tname);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void lua_pushvalue(IntPtr L, int idx);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void lua_setfield(IntPtr L, int idx, string k);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr lua_pushstring(IntPtr L, string s);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void lua_pushboolean(IntPtr L, int b);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void lua_settop(IntPtr L, int idx);

        public static void lua_pop(IntPtr L, int n) => lua_settop(L, -(n) - 1);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void lua_setglobal(IntPtr L, string name);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void lua_pushcclosure(IntPtr L, lua_CFunction fn, int n);

        public static void lua_pushcfunction(IntPtr L, lua_CFunction fn) => lua_pushcclosure(L, fn, 0);

        public static void lua_register(IntPtr L, string n, lua_CFunction fn)
        {
            lua_pushcfunction(L, fn);
            lua_setglobal(L, n);
        }

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int lua_getfield(IntPtr L, int idx, string k);

        public static int luaL_getmetatable(IntPtr L, string n)
        {
            return lua_getfield(L, LUA_REGISTRYINDEX, n);
        }

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int luaL_loadstring(IntPtr L, string s);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int lua_pcallk(IntPtr L, int nargs, int nresults, int msgh, int ctx, lua_KFunction k);

        public static int lua_pcall(IntPtr L, int nargs, int nresults, int msgh) => lua_pcallk(L, nargs, nresults, msgh, 0, null);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr lua_tolstring(IntPtr L, int idx, out int len);

        public static string lua_tostring(IntPtr L, int idx) => Marshal.PtrToStringAnsi(lua_tolstring(L, idx, out int l_length));

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int lua_gettop(IntPtr L);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int lua_type(IntPtr L, int idx);

        public static bool lua_isboolean(IntPtr L, int idx) => (lua_type(L, idx) == LUA_TBOOLEAN);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int lua_toboolean(IntPtr L, int idx);

        public static bool lua_isstring(IntPtr L, int idx) => (lua_type(L, idx) == LUA_TSTRING);

        public static bool lua_islightuserdata(IntPtr L, int idx) => (lua_type(L, idx) == LUA_TLIGHTUSERDATA);

        public static bool lua_isuserdata(IntPtr L, int idx) => (lua_type(L, idx) == LUA_TUSERDATA);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr lua_touserdata(IntPtr L, int idx);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr lua_topointer(IntPtr L, int idx);

        public static bool lua_isnumber(IntPtr L, int idx) => (lua_type(L, idx) == LUA_TNUMBER);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern double lua_tonumberx(IntPtr L, int idx, IntPtr isnum);

        public static double lua_tonumber(IntPtr L, int idx) => lua_tonumberx(L, idx, IntPtr.Zero);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr lua_newuserdatauv(IntPtr L, long sz, int nuvalue);

        public static IntPtr lua_newuserdata(IntPtr L, long sz) => lua_newuserdatauv(L, sz, 1);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void luaL_setmetatable(IntPtr L, string tname);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void lua_pushnumber(IntPtr L, double n);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int lua_isinteger(IntPtr L, int idx);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int lua_tointegerx(IntPtr L, int idx, IntPtr isnum);

        public static long lua_tointeger(IntPtr L, int idx) => lua_tointegerx(L, idx, IntPtr.Zero);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void lua_pushinteger(IntPtr L, long n);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int lua_getglobal(IntPtr L, string name);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int luaL_ref(IntPtr L, int t);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void luaL_unref(IntPtr L, int t, int iref);

        public static bool lua_isfunction(IntPtr L, int idx) => (lua_type(L, idx) == LUA_TFUNCTION);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int lua_rawgeti(IntPtr L, int idx, long n);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int lua_gc(IntPtr L, int what, long data);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void lua_pushnil(IntPtr L);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void lua_createtable(IntPtr L, int narr, int nrec);

        public static void lua_newtable(IntPtr L) => lua_createtable(L, 0, 0);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int lua_setmetatable(IntPtr L, int objindex);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int lua_getstack(IntPtr L, int level, ref lua_Debug ar);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int lua_getinfo(IntPtr L, string what, ref lua_Debug ar);

        public static bool lua_isnil(IntPtr L, int idx) => (lua_type(L, idx) == LUA_TNIL);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void lua_settable(IntPtr L, int idx);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int luaL_getmetafield(IntPtr L, int obj, string e);

        public static void lua_call(IntPtr L, int nargs, int nresults) => lua_callk(L, nargs, nresults, 0, null);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void lua_callk(IntPtr L, int nargs, int nresults, long ctx, lua_KFunction k);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void lua_rotate(IntPtr L, int idx, int n);

        public static void lua_remove(IntPtr L, int idx)
        {
            lua_rotate(L, (idx), -1);
            lua_pop(L, 1);
        }

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int lua_geti(IntPtr L, int idx, long n);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void lua_seti(IntPtr L, int idx, long n);

        [DllImport(ms_binaryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int luaL_loadbufferx(IntPtr L, byte[] buff, long sz, string name, string mode);

        public static int luaL_loadbuffer(IntPtr L, ref byte[] s, long sz, string n) => luaL_loadbufferx(L, s, sz, n, "bt");
    }
}

#pragma once

struct lua_State;

typedef int (*lua_CFunction)(lua_State* L);

/* thread status */
#define LUA_OK		0
#define LUA_YIELD	1
#define LUA_ERRRUN	2
#define LUA_ERRSYNTAX	3
#define LUA_ERRMEM	4
#define LUA_ERRERR	5

/* option for multiple returns in `lua_pcall' and `lua_call' */
#define LUA_MULTRET	(-1)

/*
** pseudo-indices
*/
#define LUA_REGISTRYINDEX	(-10000)
#define LUA_ENVIRONINDEX	(-10001)
#define LUA_GLOBALSINDEX	(-10002)

#define CREATE_LUA_FUNCTION(name, retn, ...) \
	typedef retn(*name ## _t)(__VA_ARGS__); \
	extern name ## _t name;

namespace lua
{
	CREATE_LUA_FUNCTION(loadfilex, int, lua_State*, const char*, const char*)
	CREATE_LUA_FUNCTION(pcall, int, lua_State*, int, int, int)
	CREATE_LUA_FUNCTION(tolstring, const char*, lua_State*, int, size_t*)
	CREATE_LUA_FUNCTION(pushcclosure, void, lua_State*, lua_CFunction, int)
	CREATE_LUA_FUNCTION(setfield, void, lua_State*, int, const char*)
	CREATE_LUA_FUNCTION(settop, void, lua_State*, int)
	CREATE_LUA_FUNCTION(gettop, int, lua_State*)
}

#undef CREATE_LUA_FUNCTION

namespace rww2hook
{
	namespace luafunctions
	{
		void lua_pop(lua_State* L, int n);

		bool LuaLoadFile(lua_State* L, const char* filename);

		int LuaLog(lua_State* L);
		int LuaDofile(lua_State* L);
	}
}

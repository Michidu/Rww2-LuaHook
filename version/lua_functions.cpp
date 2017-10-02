#include "lua_functions.h"

#include <sstream>

#include "Log.h"

namespace lua
{
	loadfilex_t loadfilex;
	pcall_t pcall;
	tolstring_t tolstring;
	pushcclosure_t pushcclosure;
	setfield_t setfield;
	settop_t settop;
	gettop_t gettop;
}

namespace rww2hook
{
	namespace luafunctions
	{
		void lua_pop(lua_State* L, int n)
		{
			lua::settop(L, -n - 1);
		}

		bool LuaLoadFile(lua_State* L, const char* filename)
		{
			Log::get().Write("Loading file", filename);

			int result = lua::loadfilex(L, filename, nullptr);
			if (result != LUA_OK)
			{
				size_t len;
				Log::get().Write("Lua error:", lua::tolstring(L, -1, &len));

				lua_pop(L, 1);

				return false;
			}

			result = lua::pcall(L, 0, 1, 0);
			if (result != LUA_OK)
			{
				size_t len;
				Log::get().Write("Lua error:", lua::tolstring(L, -1, &len));

				lua_pop(L, 1);

				return false;
			}

			return true;
		}

		int LuaLog(lua_State* L)
		{
			std::stringstream stream;

			const int top = lua::gettop(L);
			for (int i = 0; i < top; ++i)
			{
				size_t len;
				const char* str = lua::tolstring(L, i + 1, &len);

				stream << (i ? "  " : "") << str;
			}

			Log::get().Write("Log:", stream.str().c_str());

			return 0;
		}

		int LuaDofile(lua_State* L)
		{
			size_t length;
			const char* filename = lua::tolstring(L, 1, &length);

			LuaLoadFile(L, filename);

			return 0;
		}
	}
}

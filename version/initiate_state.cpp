#include "initiate_state.h"

#include <Windows.h>

#include <cstdio>
#include <string>

#include "MinHook.h"
#include "Log.h"
#include "lua_functions.h"
#include "signatures.h"

#define DECLARE_HOOK(name, return_type, ...) \
	typedef return_type(* name ## _Func)(__VA_ARGS__); \
	name ## _Func name ## _original; \
	return_type Hook_ ## name(__VA_ARGS__);

DECLARE_HOOK(luaL_newstate, int, void*, char, char, int)
DECLARE_HOOK(lua_close, void, lua_State*)
DECLARE_HOOK(game_update, __int64, void*, int)
DECLARE_HOOK(lua_loadx, int, lua_State*, __int64, void*, const char*, const char*)
DECLARE_HOOK(lua_setfield, void, lua_State*, int, const char*)

#undef DECLARE_HOOK

namespace rww2hook
{
	namespace
	{
		int __fastcall Hook_luaL_newstate(void* state, char a2, char a3, int a4)
		{
			static int count;
			++count;

			const int res = luaL_newstate_original(state, a2, a3, a4);

			lua_State* L = *static_cast<lua_State**>(state);

			Log::get().Write("Lua State:", L);

			if (count > 1 && (count & 1) != 0) // Third call seems right, then every second one
			{
				lua::pushcclosure(L, luafunctions::LuaLog, 0);
				lua::setfield(L, LUA_GLOBALSINDEX, "log");

				lua::pushcclosure(L, luafunctions::LuaDofile, 0);
				lua::setfield(L, LUA_GLOBALSINDEX, "dofile");
			}

			return res;
		}

		void __fastcall Hook_lua_close(lua_State* L)
		{
			Log::get().Write("Lua close:", L);

			lua_close_original(L);
		}

		__int64 __fastcall Hook_game_update(void* a1, int a2)
		{
			return game_update_original(a1, a2);
		}

		int __fastcall Hook_lua_loadx(lua_State* L, __int64 reader, void* data, const char* chunkname, const char* mode)
		{
			if (chunkname)
			{
				//Log::get().Write("Load chunk:", chunkname);

				if (!_stricmp(chunkname, "return 'g_shadow_caster'")) // Yes, load it here
				{
					Log::get().Write("Loading base scripts..");

					luafunctions::LuaLoadFile(L, "LuaMods/base/base.lua");
				}
			}

			return lua_loadx_original(L, reader, data, chunkname, mode);
		}

		void __fastcall Hook_lua_setfield(lua_State* L, int idx, const char* k)
		{
			const std::string name(k);
			if (idx == LUA_GLOBALSINDEX && (name == "xpcall" || name == "pcall")) // Don't wipe these functions
			{
				lua::settop(L, -2);
				return;
			}

			lua_setfield_original(L, idx, k);
		}

		void SetHook(const LPVOID p_target, const LPVOID p_detour, LPVOID* pp_original)
		{
			if (MH_CreateHook(p_target, p_detour, pp_original) != MH_OK)
			{
				Log::get().Write("Failed to create hook:", p_target);
				return;
			}

			if (MH_EnableHook(p_target) != MH_OK)
			{
				Log::get().Write("Failed to enable hook:", p_target);
			}
		}

		void InitHooks()
		{
			MH_Initialize();

			intptr_t dw_module_base;
			const std::vector<BYTE> rawdata = signatures::GetRawData(&dw_module_base);

			const LPVOID luaL_newstate = signatures::Search(
				rawdata, dw_module_base,
				"\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x41\x0F\xB6\xF8\x0F\xB6\xF2",
				"xxxx?xxxx?xxxxxxxxxxxx", 0, 0);
			const LPVOID lua_close = signatures::Search(
				rawdata, dw_module_base,
				"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x48\x8B\x79\x10\x48\x8B\xB7",
				"xxxx?xxxx?xxxx?xxxxxxxxxxxx", 0, 0);
			const LPVOID game_update = signatures::Search(
				rawdata, dw_module_base,
				"\x48\x83\xEC\x28\xE8\x00\x00\x00\x00\x48\x8B\x00",
				"xxxxx????xxx", 0, 0);
			const LPVOID lua_loadx = signatures::Search(
				rawdata, dw_module_base,
				"\x4C\x8B\xDC\x53\x56\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05",
				"xxxxxxxxx????xxx", 0, 0);

			SetHook(luaL_newstate, &Hook_luaL_newstate, reinterpret_cast<LPVOID*>(&luaL_newstate_original));
			SetHook(lua_close, &Hook_lua_close, reinterpret_cast<LPVOID*>(&lua_close_original));
			SetHook(game_update, &Hook_game_update, reinterpret_cast<LPVOID*>(&game_update_original));
			SetHook(lua_loadx, &Hook_lua_loadx, reinterpret_cast<LPVOID*>(&lua_loadx_original));
			SetHook(lua::setfield, &Hook_lua_setfield, reinterpret_cast<LPVOID*>(&lua_setfield_original));
		}

		void InitSignatures()
		{
			intptr_t dw_module_base;
			const std::vector<BYTE> rawdata = signatures::GetRawData(&dw_module_base);

			lua::loadfilex = static_cast<decltype(lua::loadfilex)>(signatures::Search(
				rawdata, dw_module_base,
				"\x48\x89\x5C\x24\x00\x55\x56\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05",
				"xxxx?xxxxxx????xxx", 0, 0));
			lua::pcall = static_cast<decltype(lua::pcall)>(signatures::Search(
				rawdata, dw_module_base,
				"\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x48\x8B\x59\x10\x41\x8B\xF0",
				"xxxx?xxxx?xxxxxxxxxxxx", 0, 0));
			lua::tolstring = static_cast<decltype(lua::tolstring)>(signatures::Search(
				rawdata, dw_module_base,
				"\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x49\x8B\xF8\x8B\xDA",
				"xxxx?xxxx?xxxxxxxxxx", 0, 0));
			lua::pushcclosure = static_cast<decltype(lua::pushcclosure)>(signatures::Search(
				rawdata, dw_module_base,
				"\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x4C\x8B\x49\x10\x49\x63\xF8",
				"xxxx?xxxx?xxxxxxxxxxxx", 0, 0));
			lua::setfield = static_cast<decltype(lua::setfield)>(signatures::Search(
				rawdata, dw_module_base,
				"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x4D\x8B\xD0\x48\x8B\xD9\xE8\x00\x00\x00\x00\x48\x8B\xF8",
				"xxxxxxxxxxxxxxxxx????xxx", 0, 0));
			lua::settop = static_cast<decltype(lua::settop)>(signatures::Search(
				rawdata, dw_module_base,
				"\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x85\xD2\x78\x7A",
				"xxxxxxxxxxxxx", 0, 0));
			lua::gettop = static_cast<decltype(lua::gettop)>(signatures::Search(
				rawdata, dw_module_base,
				"\x48\x8B\x41\x28\x48\x2B\x41\x20",
				"xxxxxxxx", 0, 0));
		}

		void OpenConsole()
		{
			AllocConsole();

			FILE* p_cout;
			freopen_s(&p_cout, "conout$", "w", stdout);
		}
	}

	void InitiateStates()
	{
		OpenConsole();

		Log::get().Write("Initializing..");

		InitSignatures();
		InitHooks();
	}
}

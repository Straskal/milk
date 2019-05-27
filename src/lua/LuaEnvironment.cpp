#include "LuaEnvironment.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace milk {
	namespace {
		void insertScript(
			U32 id,
			lua_State* L,
			std::unordered_map<U32, std::unordered_map<std::string, int>>& scriptidmap,
			std::vector<int>& newScripts,
			const std::string& name
		) {
			luaL_dofile(L, name.c_str());

			lua_getfield(L, -1, "awake");
			if (!lua_isnil(L, -1) && lua_isfunction(L, -1)) {
				lua_pcall(L, 0, 0, NULL);
			}

			int ref = luaL_ref(L, LUA_REGISTRYINDEX);

			std::unordered_map<U32, std::unordered_map<std::string, int>>::iterator found = scriptidmap.find(id);
			if (found != scriptidmap.end()) {
				found->second.insert(std::make_pair(name, ref));
			}
			else {
				scriptidmap.insert(std::make_pair(id, std::unordered_map<std::string, int> {
					std::make_pair(name, ref)
				}));
			}
			newScripts.push_back(ref);
		}

		void insertTickCallbacks(
			lua_State* L,
			std::vector<int>& newscripts,
			std::vector<int>& tickcallbacks
		) {
			for (int i = 0; i < newscripts.size(); ++i) {
				lua_rawgeti(L, LUA_REGISTRYINDEX, newscripts[i]);
				lua_getfield(L, -1, "tick");
				if (!lua_isnil(L, -1) && lua_isfunction(L, -1)) {
					tickcallbacks.push_back(luaL_ref(L, LUA_REGISTRYINDEX));
				}
			}
		}

		void invokeTickCallbacks(
			lua_State* L,
			std::vector<int>& tickcallbacks
		) {
			for (int i = 0; i < tickcallbacks.size(); ++i) {
				lua_rawgeti(L, LUA_REGISTRYINDEX, tickcallbacks[i]);
				if (!lua_isnil(L, -1) && lua_isfunction(L, -1)) {
					lua_pcall(L, 0, 0, NULL);
				}
			}
		}
	}
}

void milk::LuaEnvironment::init() {
	luaState_ = luaL_newstate();
	luaL_openlibs(luaState_);
}

void milk::LuaEnvironment::free() {
	lua_close(luaState_);
}

milk::MilkStartupConfig milk::LuaEnvironment::getConfiguration(const std::string& configFile) {
	// Push config table onto stack
	luaL_dofile(luaState_, configFile.c_str());

	MilkStartupConfig config;
	config.winTitle = lua::getStringField(luaState_, "title");
	config.winWidth = lua::getIntegerField(luaState_, "width");
	config.winHeight = lua::getIntegerField(luaState_, "height");
	config.resWidth = lua::getIntegerField(luaState_, "vwidth");
	config.resHeight = lua::getIntegerField(luaState_, "vheight");
	config.winFullscreen = lua::getBooleanField(luaState_, "fullscreen");

	// Pop config table off of stack
	lua_pop(luaState_, 1);

	return config;
}

void milk::LuaEnvironment::addScript(U32 id, const std::string& scriptName) {
	insertScript(id, luaState_, scriptidmap_, newscripts_, scriptName);	
}

void milk::LuaEnvironment::removeScript(U32 id, const std::string& scriptName) {
}

int milk::LuaEnvironment::getScript(U32 id, const std::string& scriptName) {
	return 0;
}

void milk::LuaEnvironment::tick() {
	insertTickCallbacks(luaState_, newscripts_, tickcallbacks_);
	newscripts_.clear();

	invokeTickCallbacks(luaState_, tickcallbacks_);
}

std::string milk::lua::getStringField(lua_State* L, const std::string& key) {
	lua_getfield(L, -1, key.c_str());
	std::string result = std::string{ (const char*)lua_tostring(L, -1) };
	lua_pop(L, 1);
	return result;
}

int milk::lua::getIntegerField(lua_State* L, const std::string& key) {
	lua_getfield(L, -1, key.c_str());
	int result = (int)lua_tointeger(L, -1);
	lua_pop(L, 1);
	return result;
}

bool milk::lua::getBooleanField(lua_State* L, const std::string& key) {
	lua_getfield(L, -1, key.c_str());
	bool result = (bool)lua_toboolean(L, -1);
	lua_pop(L, 1);
	return result;
}

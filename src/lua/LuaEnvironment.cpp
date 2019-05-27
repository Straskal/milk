#include "LuaEnvironment.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <iostream>

namespace milk {
	namespace {
		void insertScript(
			U32 id,
			lua_State* L,
			std::unordered_map<U32, std::unordered_map<std::string, int>>& scriptidmap,
			std::vector<int>& newscripts,
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
			newscripts.push_back(ref);
		}

		void insertCallbacks(
			lua_State* L,
			std::vector<int>& newscripts,
			std::vector<int>& callbacks,
			const std::string& callbackname
		) {
			for (int i = 0; i < newscripts.size(); ++i) {
				lua_rawgeti(L, LUA_REGISTRYINDEX, newscripts[i]);
				lua_getfield(L, -1, callbackname.c_str());
				if (!lua_isnil(L, -1) && lua_isfunction(L, -1)) {
					callbacks.push_back(luaL_ref(L, LUA_REGISTRYINDEX));
				}
				lua_pop(L, -1);
			}
		}

		void invokeCallbacks(
			lua_State* L,
			std::vector<int>& callbacks
		) {
			for (int i = 0; i < callbacks.size(); ++i) {
				lua_rawgeti(L, LUA_REGISTRYINDEX, callbacks[i]);
				lua_call(L, 0, 0);
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
	insertScript(id, luaState_, scriptIdMap_, newScripts_, scriptName);	
}

void milk::LuaEnvironment::removeScript(U32 id, const std::string& scriptName) {
}

int milk::LuaEnvironment::getScript(U32 id, const std::string& scriptName) {
	return 0;
}

void milk::LuaEnvironment::tick() {
	insertCallbacks(luaState_, newScripts_, tickCallbacks_, "tick");
	insertCallbacks(luaState_, newScripts_, tickCallbacks_, "postTick");
	insertCallbacks(luaState_, newScripts_, tickCallbacks_, "render");

	newScripts_.clear();

	invokeCallbacks(luaState_, tickCallbacks_);
}

void milk::LuaEnvironment::postTick() {
	invokeCallbacks(luaState_, postTickCallbacks_);
}

void milk::LuaEnvironment::render() {
	invokeCallbacks(luaState_, renderCallbacks_);
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

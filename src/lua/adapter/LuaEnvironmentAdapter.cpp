#include "LuaEnvironmentAdapter.h"
#include "LuaApi.h"
#include "LuaExtensions.h"

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
			std::vector<int>& newscripts,
			const std::string& name
		) {
			luaL_dofile(L, name.c_str());

			lua_getfield(L, -1, "awake");
			if (lua_isfunction(L, -1)) {
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
			std::vector<int>& scripts,
			std::vector<int>& callbacks,
			const std::string& callbackname
		) {
			for (int i = 0; i < scripts.size(); ++i) {
				lua_rawgeti(L, LUA_REGISTRYINDEX, scripts[i]);
				lua_getfield(L, -1, callbackname.c_str());
				if (lua_isfunction(L, -1)) {
					callbacks.push_back(luaL_ref(L, LUA_REGISTRYINDEX));
				}
				lua_pop(L, -1);
			}
		}

		void invokeCallbacks(
			lua_State* L,
			std::vector<int>& scripts,
			const std::string& callbackname
		) {
			for (int i = 0; i < scripts.size(); ++i) {
				lua_rawgeti(L, LUA_REGISTRYINDEX, scripts[i]);
				lua_getfield(L, -1, callbackname.c_str());
				if (lua_isfunction(L, -1)) {
					lua_pcall(L, 0, 0, NULL);
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
				lua_pcall(L, 0, 0, NULL);
			}
		}
	}
}

void milk::adapter::LuaEnvironmentAdapter::init(MilkState* milkState) {
	luaState_ = luaL_newstate();
	luaL_openlibs(luaState_);

	LuaApi::init(luaState_, milkState);
}

void milk::adapter::LuaEnvironmentAdapter::free() {
	lua_close(luaState_);
}

milk::MilkStartupConfig milk::adapter::LuaEnvironmentAdapter::getConfiguration(const std::string& configFile) {
	luaL_dofile(luaState_, configFile.c_str());

	MilkStartupConfig config;
	config.winTitle = luaenv::getStringField(luaState_, "title");
	config.winWidth = luaenv::getIntegerField(luaState_, "width");
	config.winHeight = luaenv::getIntegerField(luaState_, "height");
	config.resWidth = luaenv::getIntegerField(luaState_, "vwidth");
	config.resHeight = luaenv::getIntegerField(luaState_, "vheight");
	config.winFullscreen = luaenv::getBooleanField(luaState_, "fullscreen");

	lua_pop(luaState_, 1);
	return config;
}

void milk::adapter::LuaEnvironmentAdapter::addScript(U32 id, const std::string& scriptName) {
	insertScript(id, luaState_, scriptIdMap_, newScripts_, scriptName);	
}

void milk::adapter::LuaEnvironmentAdapter::tick() {
	invokeCallbacks(luaState_, tickCallbacks_);
}

void milk::adapter::LuaEnvironmentAdapter::postTick() {
	invokeCallbacks(luaState_, postTickCallbacks_);
}

void milk::adapter::LuaEnvironmentAdapter::render() {
	invokeCallbacks(luaState_, renderCallbacks_);
}

void milk::adapter::LuaEnvironmentAdapter::postRender() {
	invokeCallbacks(luaState_, newScripts_, "start");
	insertCallbacks(luaState_, newScripts_, tickCallbacks_, "tick");
	insertCallbacks(luaState_, newScripts_, tickCallbacks_, "postTick");
	insertCallbacks(luaState_, newScripts_, tickCallbacks_, "render");
	newScripts_.clear();
}

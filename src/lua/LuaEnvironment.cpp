#include "LuaEnvironment.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

void milk::LuaEnvironment::init() {
	luaState_ = luaL_newstate();
	luaL_openlibs(luaState_);
}

milk::MilkStartupConfig milk::LuaEnvironment::getConfiguration(const std::string& configFile) {
	luaL_dofile(luaState_, configFile.c_str());

	MilkStartupConfig config;
	config.winTitle = lua::getStringField(luaState_, "title");
	config.winWidth = lua::getIntegerField(luaState_, "width");
	config.winHeight = lua::getIntegerField(luaState_, "height");
	config.resWidth = lua::getIntegerField(luaState_, "vwidth");
	config.resHeight = lua::getIntegerField(luaState_, "vheight");
	config.winFullscreen = lua::getBooleanField(luaState_, "fullscreen");

	return config;
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

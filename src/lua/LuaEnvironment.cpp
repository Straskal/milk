#include "LuaEnvironment.h"
#include "LuaEnvironment.h"
#include "LuaEnvironment.h"
#include "LuaEnvironment.h"
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

void milk::LuaEnvironment::doFile(const std::string& file) {
	luaL_dofile(luaState_, file.c_str());
}

std::string milk::LuaEnvironment::getStringField(const std::string& key) {
	lua_getfield(luaState_, -1, key.c_str());
	std::string result = std::string{ (const char*)lua_tostring(luaState_, -1) };
	lua_pop(luaState_, 1);
	return result;
}

int milk::LuaEnvironment::getIntegerField(const std::string& key) {
	lua_getfield(luaState_, -1, key.c_str());
	int result = (int)lua_tointeger(luaState_, -1);
	lua_pop(luaState_, 1);
	return result;
}

bool milk::LuaEnvironment::getBooleanField(const std::string& key) {
	lua_getfield(luaState_, -1, key.c_str());
	bool result = (bool)lua_toboolean(luaState_, -1);
	lua_pop(luaState_, 1);
	return result;
}

milk::MilkStartupConfig milk::luaExtensions::getConfiguration(LuaEnvironment& env, const std::string& configFile) {
	env.doFile(configFile);

	MilkStartupConfig config;
	config.winTitle = env.getStringField("title");
	config.winWidth = env.getIntegerField("width");
	config.winHeight = env.getIntegerField("height");
	config.resWidth = env.getIntegerField("vwidth");
	config.resHeight = env.getIntegerField("vheight");
	config.winFullscreen = env.getBooleanField("fullscreen");

	return config;
}

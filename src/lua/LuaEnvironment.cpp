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

milk::MilkStartupConfig milk::LuaEnvironment::getConfiguration(const std::string& configFile) const {
	MilkStartupConfig config;

	luaL_dofile(luaState_, configFile.c_str());
	lua_getfield(luaState_, -1, "title");
	config.winTitle = std::string{ (const char*)lua_tostring(luaState_, -1) };
	lua_pop(luaState_, 1);
	lua_getfield(luaState_, -1, "width");
	config.winWidth = (int)lua_tointeger(luaState_, -1);
	lua_pop(luaState_, 1);
	lua_getfield(luaState_, -1, "height");
	config.winHeight = (int)lua_tointeger(luaState_, -1);
	lua_pop(luaState_, 1);
	lua_getfield(luaState_, -1, "vwidth");
	config.resWidth = (int)lua_tointeger(luaState_, -1);
	lua_pop(luaState_, 1);
	lua_getfield(luaState_, -1, "vheight");
	config.resHeight = (int)lua_tointeger(luaState_, -1);
	lua_pop(luaState_, 1);
	lua_getfield(luaState_, -1, "fullscreen");
	config.winFullscreen = (bool)lua_toboolean(luaState_, -1);
	lua_pop(luaState_, 1);
	lua_pop(luaState_, 1);

	return config;
}

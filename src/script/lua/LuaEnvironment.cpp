#include "LuaEnvironment.h"
#include "LuaExtensions.h"

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

void milk::lua::LuaEnvironment::init() {
	m_L = luaL_newstate();
	luaL_openlibs(m_L);
}

void milk::lua::LuaEnvironment::free() {
	lua_close(m_L);
}

milk::MilkStartupConfig milk::lua::LuaEnvironment::getConfiguration(const std::string& configFile) {
	luaL_dofile(m_L, configFile.c_str());
	MilkStartupConfig config;
	config.winTitle = luax::getStringField(m_L, "title");
	config.winWidth = luax::getIntegerField(m_L, "width");
	config.winHeight = luax::getIntegerField(m_L, "height");
	config.resWidth = luax::getIntegerField(m_L, "vwidth");
	config.resHeight = luax::getIntegerField(m_L, "vheight");
	config.winFullscreen = luax::getBooleanField(m_L, "fullscreen");
	lua_pop(m_L, 1);
	return config;
}

void milk::lua::LuaEnvironment::tick() {
}

void milk::lua::LuaEnvironment::postTick() {
}

void milk::lua::LuaEnvironment::render() {
}

void milk::lua::LuaEnvironment::postRender() {
}

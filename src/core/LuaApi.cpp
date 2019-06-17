#include "LuaApi.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "audio/lua/LuaSound.h"
#include "input/lua/LuaKeyboard.h"
#include "video/lua/LuaWindow.h"
#include "video/lua/LuaTexture.h"

static void preload_package(lua_State* L, const char* name, lua_CFunction open) {
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "preload");
	lua_pushcfunction(L, open);
	lua_setfield(L, -2, name);
	// Pop the preload table and package table off of stack
	lua_pop(L, 2);
}

void milk::LuaApi::open(lua_State* L) {
	preload_package(L, "milk.window", luaopen_window);
	preload_package(L, "milk.keyboard", luaopen_keyboard);
	preload_package(L, "milk.texture", luaopen_texture);
	preload_package(L, "milk.sound", luaopen_sound);
}

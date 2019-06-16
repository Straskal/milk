#include "LuaApi.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "input/lua/LuaKeyboard.h"
#include "video/lua/LuaWindow.h"
#include "video/lua/LuaRenderer.h"
#include "video/lua/LuaTexture.h"

namespace {
	int luaopen_video(lua_State* L) {
		lua_newtable(L);
		milk::LuaWindow::set_window_submodule(L);
		milk::LuaRenderer::set_renderer_submodule(L);
		milk::LuaTexture::set_texture_submodule(L);
		return 1;
	}

	int luaopen_input(lua_State* L) {
		lua_newtable(L);
		milk::LuaKeyboard::set_keyboard_submodule(L);
		return 1;
	}
}

void milk::LuaApi::open(lua_State* L) {
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "preload");
	lua_pushcfunction(L, luaopen_video);
	lua_setfield(L, -2, "milk.video");
	lua_pushcfunction(L, luaopen_input);
	lua_setfield(L, -2, "milk.input");
	// Pop the preload table and package table off of stack
	lua_pop(L, 2);
}

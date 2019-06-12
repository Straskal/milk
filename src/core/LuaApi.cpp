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
	void open_video_module(lua_State* L) {
		lua_newtable(L);
		milk::LuaWindow::set_window_submodule(L);
		milk::LuaRenderer::set_renderer_submodule(L);
		milk::LuaTexture::set_texture_submodule(L);
		lua_setfield(L, -2, "milk.video");
	}

	void open_input_module(lua_State* L) {
		lua_newtable(L);
		milk::LuaKeyboard::set_keyboard_submodule(L);
		lua_setfield(L, -2, "milk.input");
	}
}

void milk::LuaApi::open(lua_State* L) {
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "loaded");
	open_video_module(L);
	open_input_module(L);	
	lua_pop(L, 1);
}

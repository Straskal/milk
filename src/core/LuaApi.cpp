#include "LuaApi.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "input/lua/LuaKeyboard.h"
#include "sound/lua/LuaSound.h"
#include "video/lua/LuaWindow.h"
#include "video/lua/LuaRenderer.h"
#include "video/lua/LuaTexture.h"

static int luaopen_video(lua_State* L) {
	lua_newtable(L);
	milk::LuaWindow::pushWindowTable(L);
	lua_setfield(L, -2, "window");
	milk::LuaRenderer::pushRendererTable(L);
	lua_setfield(L, -2, "renderer");
	milk::LuaTexture::pushTextureTable(L);
	lua_setfield(L, -2, "texture");
	return 1;
}

static int luaopen_input(lua_State* L) {
	lua_newtable(L);
	milk::LuaKeyboard::pushKeyboardTable(L);
	lua_setfield(L, -2, "keyboard");
	return 1;
}

static int luaopen_sound(lua_State* L) {
	milk::LuaSound::pushSoundTable(L);
	return 1;
}

void milk::LuaApi::open(lua_State* L) {
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "preload");
	lua_pushcfunction(L, luaopen_video);
	lua_setfield(L, -2, "milk.video");
	lua_pushcfunction(L, luaopen_input);
	lua_setfield(L, -2, "milk.input");
	lua_pushcfunction(L, luaopen_sound);
	lua_setfield(L, -2, "milk.sound");
	// Pop the preload table and package table off of stack
	lua_pop(L, 2);
}

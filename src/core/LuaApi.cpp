#include "LuaApi.h"

#include "input/lua/LuaKeyboard.h"
#include "video/lua/LuaWindow.h"
#include "video/lua/LuaRenderer.h"

void milk::LuaApi::bind(lua_State* L) {
	LuaWindow::bind(L);
	LuaRenderer::bind(L);
	LuaKeyboard::bind(L);
}

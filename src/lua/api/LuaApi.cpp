#include "LuaApi.h"

#include "input/LuaKeyboard.h"
#include "window/LuaWindow.h"
#include "window/LuaRenderer.h"

void milk::LuaApi::bind(lua_State* L) {
	LuaWindow::bind(L);
	LuaRenderer::bind(L);
	LuaKeyboard::bind(L);
}

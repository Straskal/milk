#include "LuaApi.h"

#include "LuaKeyboard.h"
#include "LuaWindow.h"

void milk::LuaApi::registerApi(lua_State* L) {
	LuaWindow::bind(L);
	LuaKeyboard::bind(L);
}

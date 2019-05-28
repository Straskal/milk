#include "LuaApi.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "main/milk.h"
#include "window/Window.h"

milk::MilkState* milk::LuaApi::milkState_ = nullptr;

void milk::LuaApi::init(lua_State* L, milk::MilkState* milkState) {
	milkState_ = milkState;
}

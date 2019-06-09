#include "LuaRenderer.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "core/Locator.h"
#include "graphics/Color.h"
#include "math/Rectangle.h"
#include "lua/lua_extensions.h"

namespace {
	int draw_rect(lua_State* L) {
		int x = lua_tointeger(L, 1);
		int y = lua_tointeger(L, 2);
		int w = lua_tointeger(L, 3);
		int h = lua_tointeger(L, 4);

		milk::Locator::renderer->drawRectangle(milk::Rectangle{ x , y, w, h }, milk::Color::red());
		return 0;
	}

	int set_resolution(lua_State* L) {
		int w = luaL_checkinteger(L, 1);
		int h = luaL_checkinteger(L, 2);
		milk::Locator::renderer->resolution(w, h);
		return 0;
	}

	static const luaL_Reg lib[] = {
		{ "draw_rect", draw_rect },
		{ "set_resolution", set_resolution },
		{ NULL, NULL }
	};
}

void milk::LuaRenderer::bind(lua_State* L) {
	luaM::register_module(L, "milk.renderer", lib);
}
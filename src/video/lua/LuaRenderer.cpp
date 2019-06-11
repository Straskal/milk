#include "LuaRenderer.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "core/Locator.h"
#include "core/lua_extensions.h"
#include "math/Rectangle.h"
#include "video/Color.h"
#include "video/Texture.h"

namespace {
	int draw_rect(lua_State* L) {
		int x = lua_tointeger(L, 1);
		int y = lua_tointeger(L, 2);
		int w = lua_tointeger(L, 3);
		int h = lua_tointeger(L, 4);

		milk::Locator::renderer->drawRectangle(milk::Rectangle{ x , y, w, h }, milk::Color::red());
		return 0;
	}

	int draw(lua_State* L) {
		milk::TextureHandle* handle = (milk::TextureHandle*)luaL_checkudata(L, 1, "milk.texturehandle");
		int x = lua_tointeger(L, 2);
		int y = lua_tointeger(L, 3);

		milk::Rectangle src = { 0, 0, 64, 64 };
		milk::Rectangle dst = { x, y, 64, 64 };

		milk::Locator::renderer->draw(*handle->texture, src, dst, 0);
		return 0;
	}

	int set_resolution(lua_State* L) {
		int w = luaL_checkinteger(L, 1);
		int h = luaL_checkinteger(L, 2);
		milk::Locator::renderer->resolution(w, h);
		return 0;
	}

	static const luaL_Reg lib[] = {
		{ "draw", draw },
		{ "draw_rect", draw_rect },
		{ "set_resolution", set_resolution },
		{ NULL, NULL }
	};
}

void milk::LuaRenderer::set_renderer_submodule(lua_State* L) {
	luaL_newlib(L, lib);
	lua_setfield(L, -2, "renderer");
}

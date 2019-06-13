#include "LuaRenderer.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "LuaTexture.h"
#include "core/Locator.h"
#include "core/lua_extensions.h"
#include "math/Rectangle.h"
#include "video/Color.h"
#include "video/Texture.h"

namespace {
	int draw(lua_State* L) {
		milk::TextureHandle* handle = (milk::TextureHandle*)luaL_checkudata(L, 1, milk::LuaTexture::METATABLE);
		milk::Texture* tex = handle->texture;

		// draw(texture, pos)
		if (lua_gettop(L) == 2) {
			int w = tex->width;
			int h = tex->height;
			int x = milk::luaM::get_int_field(L, 2, "x");
			int y = milk::luaM::get_int_field(L, 2, "y");

			milk::Rectangle src = { 0, 0, w, h };
			milk::Rectangle dst = { x, y, w, h };
			milk::Locator::renderer->draw(handle->texture, &src, &dst, 0);
		}
		// draw(texture, pos, srcrect)
		else if (lua_gettop(L) == 3) {
			int posx = milk::luaM::get_int_field(L, 2, "x");
			int posy = milk::luaM::get_int_field(L, 2, "y");

			int rectx = milk::luaM::get_int_field(L, 3, "x");
			int recty = milk::luaM::get_int_field(L, 3, "y");
			int rectw = milk::luaM::get_int_field(L, 3, "w");
			int recth = milk::luaM::get_int_field(L, 3, "h");

			milk::Rectangle src = { rectx, recty, rectw, recth };
			milk::Rectangle dst = { posx, posy, rectw, recth };
			milk::Locator::renderer->draw(handle->texture, &src, &dst, 0);
		}		
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
		{ "set_resolution", set_resolution },
		{ NULL, NULL }
	};
}

void milk::LuaRenderer::set_renderer_submodule(lua_State* L) {
	luaL_newlib(L, lib); 
	lua_setfield(L, -2, "renderer");
}

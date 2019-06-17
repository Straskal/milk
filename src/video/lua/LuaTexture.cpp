#include "LuaTexture.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "video/Texture.h"
#include "core/Locator.h"
#include "core/lua_extensions.h"
#include "math/Rectangle.h"
#include "video/Color.h"

const char* METATABLE = "milk.texturehandle";

static int new_texture(lua_State* L) {
	if (lua_isstring(L, 1)) {
		const char* value = lua_tostring(L, 1);
		milk::Texture* texture = milk::Locator::textures->load(value);
		if (texture != nullptr) {
			milk::TextureHandle* handle = (milk::TextureHandle*)lua_newuserdata(L, sizeof(milk::TextureHandle*));
			luaL_getmetatable(L, METATABLE);
			lua_setmetatable(L, -2);
			handle->texture = texture;
			lua_pushboolean(L, true);
			return 2;
		}
	}
	lua_pushnil(L);
	lua_pushboolean(L, false);
	return 2;
}

static const luaL_Reg funcs[] = {
	{ "new", new_texture },
	{ NULL, NULL }
};

static int gc(lua_State* L) {
	milk::TextureHandle* handle = (milk::TextureHandle*)luaL_checkudata(L, 1, METATABLE);
	milk::Locator::textures->dereference(handle->texture);
	return 0;
}

static int draw(lua_State* L) {
	milk::TextureHandle* handle = (milk::TextureHandle*)luaL_checkudata(L, 1, METATABLE);
	milk::Texture* tex = handle->texture;

	int w = tex->width;
	int h = tex->height;
	int x = milk::luaM::get_int_field(L, 2, "x");
	int y = milk::luaM::get_int_field(L, 2, "y");

	milk::Rectangle src = { 0, 0, w, h };
	milk::Rectangle dst = { x, y, w, h };
	milk::Locator::renderer->draw(handle->texture, &src, &dst, 0);
	return 0;
}

static int drawex(lua_State* L) {
	milk::TextureHandle* handle = (milk::TextureHandle*)luaL_checkudata(L, 1, METATABLE);
	milk::Texture* tex = handle->texture;

	int posx = milk::luaM::get_int_field(L, 2, "x");
	int posy = milk::luaM::get_int_field(L, 2, "y");

	int rectx = milk::luaM::get_int_field(L, 3, "x");
	int recty = milk::luaM::get_int_field(L, 3, "y");
	int rectw = milk::luaM::get_int_field(L, 3, "w");
	int recth = milk::luaM::get_int_field(L, 3, "h");

	milk::Rectangle src = { rectx, recty, rectw, recth };
	milk::Rectangle dst = { posx, posy, rectw, recth };
	milk::Locator::renderer->draw(handle->texture, &src, &dst, 0);
	return 0;
}

static int get_size(lua_State* L) {
	milk::TextureHandle* handle = (milk::TextureHandle*)luaL_checkudata(L, 1, METATABLE);
	lua_pushinteger(L, handle->texture->width);
	lua_pushinteger(L, handle->texture->height);
	return 2;
}

static const luaL_Reg meta_funcs[] = {
	{ "__gc", gc },
	{ "draw", draw },
	{ "drawex", drawex },
	{ "get_size", get_size },
	{ NULL, NULL }
};

int milk::luaopen_texture(lua_State* L) {
	luaL_newmetatable(L, METATABLE);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, meta_funcs, 0);
	lua_pop(L, 1); // Pop metatable off of stack
	luaL_newlib(L, funcs);
	return 1;
}

#include "LuaTexture.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "video/Texture.h"
#include "core/Locator.h"

const char* milk::LuaTexture::METATABLE = "milk.texturehandle";

static int new_texture(lua_State* L) {
	if (lua_isstring(L, 1)) {
		const char* value = lua_tostring(L, 1);
		milk::Texture* texture = milk::Locator::textures->load(value);
		if (texture != nullptr) {
			milk::TextureHandle* handle = (milk::TextureHandle*)lua_newuserdata(L, sizeof(milk::TextureHandle*));
			luaL_getmetatable(L, milk::LuaTexture::METATABLE);
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
	milk::TextureHandle* handle = (milk::TextureHandle*)luaL_checkudata(L, 1, milk::LuaTexture::METATABLE);
	milk::Locator::textures->dereference(handle->texture);
	return 0;
}

static int get_size(lua_State* L) {
	milk::TextureHandle* handle = (milk::TextureHandle*)luaL_checkudata(L, 1, milk::LuaTexture::METATABLE);
	lua_pushinteger(L, handle->texture->width);
	lua_pushinteger(L, handle->texture->height);
	return 2;
}

static const luaL_Reg meta_funcs[] = {
	{ "__gc", gc },
	{ "get_size", get_size },
	{ NULL, NULL }
};

void milk::LuaTexture::pushTextureTable(lua_State* L) {
	luaL_newmetatable(L, METATABLE);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, meta_funcs, 0);
	lua_pop(L, 1); // Pop metatable off of stack
	luaL_newlib(L, funcs);
}

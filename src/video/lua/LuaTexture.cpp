#include "LuaTexture.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "video/Texture.h"
#include "core/Locator.h"

const static char* TEXTURE_HANDLE_METATABLE = "milk.texturehandle";

namespace {
	int new_texture(lua_State* L) {
		if (lua_isstring(L, 1)) {
			const char* value = lua_tostring(L, 1);
			milk::Texture* texture = milk::Locator::textures->reference(value);
			if (texture != nullptr) {
				milk::TextureHandle* handle = (milk::TextureHandle*)lua_newuserdata(L, sizeof(milk::TextureHandle*));
				luaL_getmetatable(L, TEXTURE_HANDLE_METATABLE);
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

	const luaL_Reg lib[] = {
		{ "new", new_texture },
		{ NULL, NULL }
	};

	int gc(lua_State* L) {
		milk::TextureHandle* handle = (milk::TextureHandle*)luaL_checkudata(L, 1, TEXTURE_HANDLE_METATABLE);
		milk::Locator::textures->dereference(handle->texture);
		return 0;
	}

	int get_size(lua_State* L) {
		milk::TextureHandle* handle = (milk::TextureHandle*)luaL_checkudata(L, 1, TEXTURE_HANDLE_METATABLE);
		lua_pushinteger(L, handle->texture->width);
		lua_pushinteger(L, handle->texture->height);
		return 2;
	}

	const luaL_Reg handle_m[] = {
		{ "__gc", gc },
		{ "get_size", get_size },
		{ NULL, NULL }
	};
}

void milk::LuaTexture::set_texture_submodule(lua_State* L) {
	luaL_newmetatable(L, TEXTURE_HANDLE_METATABLE);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, handle_m, 0);
	lua_pop(L, 1); // Pop meta table off of stack

	luaL_newlib(L, lib);
	lua_setfield(L, -2, "texture");
}

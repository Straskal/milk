#include "luaopen_milk_graphics.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "Color.h"
#include "Renderer.h"
#include "Texture.h"
#include "TextureCache.h"
#include "core/Locator.h"
#include "core/luamlib.h"
#include "math/Rectangle.h"

static const char* TEXTURE_METATABLE = "milk.texture";

static milk::Color draw_color = { 0xFF, 0xFF, 0xFF, 0xFF };
static milk::Rectangle source_rect = { 0, 0, 0, 0};
static milk::Rectangle dest_rect = { 0, 0, 0, 0 };

static int texturemeta_gc(lua_State* L) {
	milk::Texture* texture = (milk::Texture*)luaL_checkudata(L, 1, TEXTURE_METATABLE);
	milk::Locator::textures->dereference(texture->data);
	return 0;
}

static int texturemeta_get_path(lua_State* L) {
	milk::Texture* texture = (milk::Texture*)luaL_checkudata(L, 1, TEXTURE_METATABLE);
	const char* path = texture->data->path.c_str();
	lua_pushstring(L, path);
	return 1;
}

static int texturemeta_get_ref_count(lua_State* L) {
	milk::Texture* texture = (milk::Texture*)luaL_checkudata(L, 1, TEXTURE_METATABLE);
	int refCount = texture->data->refCount;
	lua_pushinteger(L, refCount);
	return 1;
}

static int texturemeta_get_size(lua_State* L) {
	milk::Texture* texture = (milk::Texture*)luaL_checkudata(L, 1, TEXTURE_METATABLE);
	lua_pushinteger(L, texture->data->width);
	lua_pushinteger(L, texture->data->height);
	return 2;
}

static const luaL_Reg texturemeta_funcs[] = {
	{ "__gc", texturemeta_gc },
	{ "get_path", texturemeta_get_path },
	{ "get_ref_count", texturemeta_get_ref_count },
	{ "get_size", texturemeta_get_size },
	{ NULL, NULL }
};

static int graphics_new_texture(lua_State* L) {
	if (lua_isstring(L, 1)) {
		const char* value = lua_tostring(L, 1);
		milk::TextureData* textureData = milk::Locator::textures->load(value);
		if (textureData != nullptr) {
			milk::Texture* texture = (milk::Texture*)lua_newuserdata(L, sizeof(milk::Texture*));
			luaL_getmetatable(L, TEXTURE_METATABLE);
			lua_setmetatable(L, -2);
			texture->data = textureData;
			lua_pushboolean(L, true);
			return 2;
		}
	}
	lua_pushnil(L);
	lua_pushboolean(L, false);
	return 2;
}

static int graphics_set_virtual_resolution(lua_State* L) {
	int w = luaL_checkinteger(L, 1);
	int h = luaL_checkinteger(L, 2);
	milk::Locator::renderer->resolution(w, h);
	return 0;
}

static int graphics_set_draw_color(lua_State* L) {
	milk::u8 r = (milk::u8)(luaL_checknumber(L, 1) * 255);
	milk::u8 b = (milk::u8)(luaL_checknumber(L, 2) * 255);
	milk::u8 g = (milk::u8)(luaL_checknumber(L, 3) * 255);
	milk::u8 a = (milk::u8)(luaL_checknumber(L, 4) * 255);

	draw_color.r = r;
	draw_color.b = b;
	draw_color.g = g;
	draw_color.a = a;

	milk::Locator::renderer->setDrawColor(&draw_color);
	return 0;
}

static int graphics_draw(lua_State* L) {
	milk::Texture* texture = (milk::Texture*)luaL_checkudata(L, 1, TEXTURE_METATABLE);
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);
	int flip = luaL_optinteger(L, 4, milk::NO_FLIP);

	milk::TextureData* data = texture->data;
	int w = data->width;
	int h = data->height;

	source_rect.x = 0;
	source_rect.y = 0;
	source_rect.width = w;
	source_rect.height = h;

	dest_rect.x = x;
	dest_rect.y = y;
	dest_rect.width = w;
	dest_rect.height = h;

	milk::Locator::renderer->draw(texture, &source_rect, &dest_rect, (milk::u8)flip);
	return 0;
}

static int graphics_drawx(lua_State* L) {
	milk::Texture* texture = (milk::Texture*)luaL_checkudata(L, 1, TEXTURE_METATABLE);
	int posx = luaL_checkinteger(L, 2);
	int posy = luaL_checkinteger(L, 3);
	int rectx = luaL_checkinteger(L, 4);
	int recty = luaL_checkinteger(L, 5);
	int rectw = luaL_checkinteger(L, 6);
	int recth = luaL_checkinteger(L, 7);
	int scale = luaL_checkinteger(L, 8);
	int flip = luaL_optinteger(L, 9, milk::NO_FLIP);

	source_rect.x = rectx;
	source_rect.y = recty;
	source_rect.width = rectw;
	source_rect.height = recth;

	dest_rect.x = posx;
	dest_rect.y = posy;
	dest_rect.width = rectw * scale;
	dest_rect.height = recth * scale;

	milk::Locator::renderer->draw(texture, &source_rect, &dest_rect, (milk::u8)flip);
	return 0;
}

static int graphics_draw_rect(lua_State* L) {
	int rectx = luaL_checkinteger(L, 1);
	int recty = luaL_checkinteger(L, 2);
	int rectw = luaL_checkinteger(L, 3);
	int recth = luaL_checkinteger(L, 4);

	dest_rect.x = rectx;
	dest_rect.y = recty;
	dest_rect.width = rectw;
	dest_rect.height = recth;

	milk::Locator::renderer->drawRectangle(&dest_rect);
	return 0;
}

static int graphics_draw_filled_rect(lua_State* L) {
	int rectx = luaL_checkinteger(L, 1);
	int recty = luaL_checkinteger(L, 2);
	int rectw = luaL_checkinteger(L, 3);
	int recth = luaL_checkinteger(L, 4);

	dest_rect.x = rectx;
	dest_rect.y = recty;
	dest_rect.width = rectw;
	dest_rect.height = recth;

	milk::Locator::renderer->drawRectangleFilled(&dest_rect);
	return 0;
}

static const luaL_Reg graphics_funcs[] = {
	{ "new_texture", graphics_new_texture },
	{ "set_virtual_resolution", graphics_set_virtual_resolution },
	{ "set_draw_color", graphics_set_draw_color },
	{ "draw", graphics_draw },
	{ "drawx", graphics_drawx },
	{ "draw_rect", graphics_draw_rect },
	{ "draw_filled_rect", graphics_draw_filled_rect },
	{ NULL, NULL }
};

static const milk::luaM_Enum flip_enum[] = {
	{ "NONE", milk::FlipFlags::NO_FLIP },
	{ "X", milk::FlipFlags::FLIP_X },
	{ "Y", milk::FlipFlags::FLIP_Y }
};

int milk::luaopen_milk_graphics(lua_State* L) {
	luaM_createmetatable(L, TEXTURE_METATABLE, texturemeta_funcs);
	luaL_newlib(L, graphics_funcs);
	luaM_setenumfield(L, -1, "flip_flags", flip_enum, sizeof(flip_enum) / sizeof(luaM_Enum));
	return 1;
}

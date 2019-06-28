#include "luaopen_milk_graphics.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "Color.h"
#include "Image.h"
#include "ImageCache.h"
#include "Renderer.h"
#include "core/Locator.h"
#include "core/luamlib.h"
#include "math/Rectangle.h"

static const char* IMAGE_METATABLE = "milk.image";

static milk::Color draw_color = { 0xFF, 0xFF, 0xFF, 0xFF };
static milk::Rectangle source_rect = { 0, 0, 0, 0 };
static milk::RectangleF dest_rect = { 0, 0, 0, 0 };

static int imagemeta_tostring(lua_State* L)
{
	milk::Image* image = (milk::Image*)luaL_checkudata(L, 1, IMAGE_METATABLE);
	const char* path = image->data->path.c_str();
	lua_pushstring(L, path);
	return 1;
}

static int imagemeta_gc(lua_State* L)
{
	milk::Image* image = (milk::Image*)luaL_checkudata(L, 1, IMAGE_METATABLE);
	milk::Locator::images->dereference(image->data);
	return 0;
}

static int imagemeta_get_size(lua_State* L)
{
	milk::Image* image = (milk::Image*)luaL_checkudata(L, 1, IMAGE_METATABLE);
	lua_pushinteger(L, image->data->width);
	lua_pushinteger(L, image->data->height);
	return 2;
}

static const luaL_Reg imagemeta_funcs[] = {
	{ "__tostring", imagemeta_tostring },
	{ "__gc", imagemeta_gc },
	{ "get_size", imagemeta_get_size },
	{ NULL, NULL }
};

static int graphics_new_image(lua_State* L)
{
	if (lua_isstring(L, 1)) {
		const char* value = lua_tostring(L, 1);
		milk::ImageData* imageData = milk::Locator::images->load(value);
		if (imageData != nullptr) {
			milk::Image* image = (milk::Image*)lua_newuserdata(L, sizeof(milk::Image));
			luaL_getmetatable(L, IMAGE_METATABLE);
			lua_setmetatable(L, -2);
			image->data = imageData;
			lua_pushboolean(L, true);
			return 2;
		}
	}
	lua_pushnil(L);
	lua_pushboolean(L, false);
	return 2;
}

static int graphics_set_virtual_resolution(lua_State* L)
{
	int w = (int)luaL_checkinteger(L, 1);
	int h = (int)luaL_checkinteger(L, 2);

	milk::Locator::renderer->resolution(w, h);
	return 0;
}

static int graphics_set_draw_color(lua_State* L)
{
	draw_color.r = (milk::u8)(luaL_checknumber(L, 1) * 255);
	draw_color.b = (milk::u8)(luaL_checknumber(L, 2) * 255);
	draw_color.g = (milk::u8)(luaL_checknumber(L, 3) * 255);
	draw_color.a = (milk::u8)(luaL_checknumber(L, 4) * 255);

	milk::Locator::renderer->setDrawColor(&draw_color);
	return 0;
}

static int graphics_draw(lua_State* L)
{
	milk::Image* image = (milk::Image*)luaL_checkudata(L, 1, IMAGE_METATABLE);
	float x = (float)luaL_checknumber(L, 2);
	float y = (float)luaL_checknumber(L, 3);
	int flip = (int)luaL_optinteger(L, 4, milk::NO_FLIP);

	milk::ImageData* data = image->data;
	int w = data->width;
	int h = data->height;

	source_rect.x = 0;
	source_rect.y = 0;
	source_rect.w = w;
	source_rect.h = h;

	dest_rect.x = x;
	dest_rect.y = y;
	dest_rect.w = (float)w;
	dest_rect.h = (float)h;

	milk::Locator::renderer->draw(image, &source_rect, &dest_rect, (milk::u8)flip);
	return 0;
}

static int graphics_drawx(lua_State* L)
{
	milk::Image* image = (milk::Image*)luaL_checkudata(L, 1, IMAGE_METATABLE);
	float posx = (float)luaL_checknumber(L, 2);
	float posy = (float)luaL_checknumber(L, 3);
	int rectx = (int)luaL_checkinteger(L, 4);
	int recty = (int)luaL_checkinteger(L, 5);
	int rectw = (int)luaL_checkinteger(L, 6);
	int recth = (int)luaL_checkinteger(L, 7);
	float scale = (float)luaL_checknumber(L, 8);
	int flip = (int)luaL_optinteger(L, 9, milk::NO_FLIP);

	source_rect.x = rectx;
	source_rect.y = recty;
	source_rect.w = rectw;
	source_rect.h = recth;

	dest_rect.x = posx;
	dest_rect.y = posy;
	dest_rect.w = (float)rectw * scale;
	dest_rect.h = (float)recth * scale;

	milk::Locator::renderer->draw(image, &source_rect, &dest_rect, (milk::u8)flip);
	return 0;
}

static int graphics_draw_rect(lua_State* L)
{
	dest_rect.x = (float)luaL_checknumber(L, 1);
	dest_rect.y = (float)luaL_checknumber(L, 2);
	dest_rect.w = (float)luaL_checknumber(L, 3);
	dest_rect.h = (float)luaL_checknumber(L, 4);

	milk::Locator::renderer->drawRectangle(&dest_rect);
	return 0;
}

static int graphics_draw_filled_rect(lua_State* L)
{
	dest_rect.x = (float)luaL_checknumber(L, 1);
	dest_rect.y = (float)luaL_checknumber(L, 2);
	dest_rect.w = (float)luaL_checknumber(L, 3);
	dest_rect.h = (float)luaL_checknumber(L, 4);

	milk::Locator::renderer->drawRectangleFilled(&dest_rect);
	return 0;
}

static const luaL_Reg graphics_funcs[] = {
	{ "new_image", graphics_new_image },
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

int milk::luaopen_milk_graphics(lua_State* L)
{
	luaM_createmetatable(L, IMAGE_METATABLE, imagemeta_funcs);
	luaL_newlib(L, graphics_funcs);
	luaM_setenumfield(L, -1, "flip_flags", flip_enum, sizeof(flip_enum) / sizeof(luaM_Enum));
	return 1;
}

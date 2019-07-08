#include "luaopen_milk_graphics.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "graphics.h"
#include "image.h"
#include "luamlib.h"
#include "uid.h"

static const char* IMAGE_METATABLE = "milk.image";

static int lua_imagemeta_tostring(lua_State* L)
{
	milk::Image* image = (milk::Image*)luaL_checkudata(L, 1, IMAGE_METATABLE);
	milk::ImageData* imageData = milk::graphics_get_imagedata(image->uid);
	const char* path = imageData->path.c_str();
	lua_pushstring(L, path);
	return 1;
}

static int lua_imagemeta_gc(lua_State* L)
{
	milk::Image* image = (milk::Image*)luaL_checkudata(L, 1, IMAGE_METATABLE);
	milk::graphics_dereference_imagedata(image->uid);
	return 0;
}

static int lua_imagemeta_get_size(lua_State* L)
{
	milk::Image* image = (milk::Image*)luaL_checkudata(L, 1, IMAGE_METATABLE);
	milk::ImageData* imageData = milk::graphics_get_imagedata(image->uid);
	lua_pushinteger(L, imageData->width);
	lua_pushinteger(L, imageData->height);
	return 2;
}

static const luaL_Reg lua_imagemeta_funcs[] = {
	{ "__tostring", lua_imagemeta_tostring },
	{ "__gc", lua_imagemeta_gc },
	{ "get_size", lua_imagemeta_get_size },
	{ nullptr, nullptr }
};

static int lua_graphics_new_image(lua_State* L)
{
	const char* path = (const char*)luaL_checkstring(L, 1);
	milk::u32 uid = milk::graphics_load_imagedata(path);
	if (uid != milk::INVALID_UID) {
		milk::Image* image = (milk::Image*)lua_newuserdata(L, sizeof(milk::Image));
		image->uid = uid;
		luaL_getmetatable(L, IMAGE_METATABLE);
		lua_setmetatable(L, -2);
		lua_pushboolean(L, true);
		return 2;
	}
	return luaL_error(L, "could not load image file: %s", path);
}

static int lua_graphics_set_resolution(lua_State* L)
{
	int w = (int)luaL_checkinteger(L, 1);
	int h = (int)luaL_checkinteger(L, 2);

	milk::graphics_set_resolution(w, h);
	return 0;
}

static int lua_graphics_get_resolution(lua_State* L)
{
	std::tuple<int, int> resolution = milk::graphics_get_resolution();
	int w = std::get<0>(resolution);
	int h = std::get<1>(resolution);

	lua_pushinteger(L, w);
	lua_pushinteger(L, h);
	return 2;
}

static int lua_graphics_clear(lua_State* L)
{
	(void)L;

	milk::graphics_clear();
	return 0;
}

static int lua_graphics_set_draw_color(lua_State* L)
{
	double r = (double)luaL_checknumber(L, 1);
	double g = (double)luaL_checknumber(L, 2);
	double b = (double)luaL_checknumber(L, 3);
	double a = (double)luaL_checknumber(L, 4);

	milk::graphics_set_draw_color(r, g, b, a);
	return 0;
}

static int lua_graphics_draw_rect(lua_State* L)
{
	float x = (float)luaL_checknumber(L, 1);
	float y = (float)luaL_checknumber(L, 2);
	float w = (float)luaL_checknumber(L, 3);
	float h = (float)luaL_checknumber(L, 4);

	milk::graphics_draw_rectangle(x, y, w, h);
	return 0;
}

static int lua_graphics_draw_filled_rect(lua_State* L)
{
	float x = (float)luaL_checknumber(L, 1);
	float y = (float)luaL_checknumber(L, 2);
	float w = (float)luaL_checknumber(L, 3);
	float h = (float)luaL_checknumber(L, 4);

	milk::graphics_draw_rectangle_filled(x, y, w, h);
	return 0;
}

static int lua_graphics_draw(lua_State* L)
{
	milk::Image* image = (milk::Image*)luaL_checkudata(L, 1, IMAGE_METATABLE);
	float x = (float)luaL_checknumber(L, 2);
	float y = (float)luaL_checknumber(L, 3);

	milk::graphics_draw(image, x, y);
	return 0;
}

static int lua_graphics_drawx(lua_State* L)
{
	milk::Image* image = (milk::Image*)luaL_checkudata(L, 1, IMAGE_METATABLE);
	float posx = (float)luaL_checknumber(L, 2);
	float posy = (float)luaL_checknumber(L, 3);
	int rectx = (int)luaL_checkinteger(L, 4);
	int recty = (int)luaL_checkinteger(L, 5);
	int rectw = (int)luaL_checkinteger(L, 6);
	int recth = (int)luaL_checkinteger(L, 7);
	float scalex = (float)luaL_checknumber(L, 8);
	float scaley = (float)luaL_checknumber(L, 9);
	double angle = (double)luaL_checknumber(L, 10);

	milk::graphics_draw(image, posx, posy, rectx, recty, rectw, recth, scalex, scaley, angle);
	return 0;
}

static int lua_graphics_present(lua_State* L)
{
	(void)L;

	milk::graphics_present();
	return 0;
}

static const luaL_Reg graphics_funcs[] = {
	{ "new_image", lua_graphics_new_image },
	{ "get_resolution", lua_graphics_get_resolution },
	{ "set_resolution", lua_graphics_set_resolution },
	{ "clear", lua_graphics_clear },
	{ "set_draw_color", lua_graphics_set_draw_color },
	{ "draw_rect", lua_graphics_draw_rect },
	{ "draw_filled_rect", lua_graphics_draw_filled_rect },
	{ "draw", lua_graphics_draw },
	{ "drawx", lua_graphics_drawx },
	{ "present", lua_graphics_present },
	{ nullptr, nullptr }
};

int luaopen_milk_graphics(lua_State* L)
{
	milk::luaM_createmetatable(L, IMAGE_METATABLE, lua_imagemeta_funcs);
	luaL_newlib(L, graphics_funcs);
	return 1;
}

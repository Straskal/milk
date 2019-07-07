#include "luaopen_milk_graphics.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "Image.h"
#include "ImageCache.h"
#include "luamlib.h"
#include "Renderer.h"
#include "State.h"

static const char* IMAGE_METATABLE = "milk.image";

static int imagemeta_tostring(lua_State* L)
{
	milk::Image* image = (milk::Image*)luaL_checkudata(L, 1, IMAGE_METATABLE);
	const char* path = image->data->path.c_str();
	lua_pushstring(L, path);
	return 1;
}

static int imagemeta_gc(lua_State* L)
{
	if (milk::State::initialized) {
		milk::Image* image = (milk::Image*)luaL_checkudata(L, 1, IMAGE_METATABLE);
		milk::State::images->dereference(image->data);
	}
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
	{ nullptr, nullptr }
};

static int graphics_new_image(lua_State* L)
{
	if (lua_isstring(L, 1)) {
		const char* value = lua_tostring(L, 1);
		milk::ImageData* imageData = milk::State::images->load(value);
		if (imageData != nullptr) {
			milk::Image* image = (milk::Image*)lua_newuserdata(L, sizeof(milk::Image));
			image->data = imageData;
			luaL_getmetatable(L, IMAGE_METATABLE);
			lua_setmetatable(L, -2);
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

	milk::State::renderer->resolution(w, h);
	return 0;
}

static int graphics_get_virtual_resolution(lua_State* L)
{
	std::tuple<int, int> resolution = milk::State::renderer->resolution();
	int w = std::get<0>(resolution);
	int h = std::get<1>(resolution);

	lua_pushinteger(L, w);
	lua_pushinteger(L, h);
	return 2;
}

static int graphics_clear(lua_State* L) 
{
	(void)L;

	milk::State::renderer->clear();
	return 0;
}

static int graphics_set_draw_color(lua_State* L)
{
	double r = (double)luaL_checknumber(L, 1);
	double g = (double)luaL_checknumber(L, 2);
	double b = (double)luaL_checknumber(L, 3);
	double a = (double)luaL_checknumber(L, 4);

	milk::State::renderer->setDrawColor(r, g, b, a);
	return 0;
}

static int graphics_draw_rect(lua_State* L)
{
	float x = (float)luaL_checknumber(L, 1);
	float y = (float)luaL_checknumber(L, 2);
	float w = (float)luaL_checknumber(L, 3);
	float h = (float)luaL_checknumber(L, 4);

	milk::State::renderer->drawRectangle(x, y, w, h);
	return 0;
}

static int graphics_draw_filled_rect(lua_State* L)
{
	float x = (float)luaL_checknumber(L, 1);
	float y = (float)luaL_checknumber(L, 2);
	float w = (float)luaL_checknumber(L, 3);
	float h = (float)luaL_checknumber(L, 4);

	milk::State::renderer->drawRectangleFilled(x, y, w, h);
	return 0;
}

static int graphics_draw(lua_State* L)
{
	milk::Image* image = (milk::Image*)luaL_checkudata(L, 1, IMAGE_METATABLE);
	float x = (float)luaL_checknumber(L, 2);
	float y = (float)luaL_checknumber(L, 3);

	milk::State::renderer->draw(image, x, y);
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
	float scalex = (float)luaL_checknumber(L, 8);
	float scaley = (float)luaL_checknumber(L, 9);
	double angle = (double)luaL_checknumber(L, 10);

	milk::State::renderer->draw(image, posx, posy, rectx, recty, rectw, recth, scalex, scaley, angle);
	return 0;
}

static int graphics_present(lua_State* L)
{
	(void)L;

	milk::State::renderer->present();
	return 0;
}

static const luaL_Reg graphics_funcs[] = {
	{ "new_image", graphics_new_image },
	{ "get_virtual_resolution", graphics_get_virtual_resolution },
	{ "set_virtual_resolution", graphics_set_virtual_resolution },
	{ "clear", graphics_clear },
	{ "set_draw_color", graphics_set_draw_color },
	{ "draw_rect", graphics_draw_rect },
	{ "draw_filled_rect", graphics_draw_filled_rect },
	{ "draw", graphics_draw },
	{ "drawx", graphics_drawx },
	{ "present", graphics_present },
	{ nullptr, nullptr }
};

int luaopen_milk_graphics(lua_State* L)
{
	milk::luaM_createmetatable(L, IMAGE_METATABLE, imagemeta_funcs);
	luaL_newlib(L, graphics_funcs);
	return 1;
}

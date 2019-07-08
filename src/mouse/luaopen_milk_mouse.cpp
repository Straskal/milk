#include "luaopen_milk_mouse.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "Mouse.h"
#include "luamlib.h"
#include "window/window.h"

#include "graphics/graphics.h"

static int lua_mouse_is_button_down(lua_State* L)
{
	milk::MouseButtons button = (milk::MouseButtons)luaL_checkinteger(L, 1);
	bool down = milk::mouse_is_button_down(button);
	lua_pushboolean(L, down);
	return 1;
}

static int lua_mouse_is_button_pressed(lua_State* L)
{
	milk::MouseButtons button = (milk::MouseButtons)luaL_checkinteger(L, 1);
	bool pressed = milk::mouse_is_button_pressed(button);
	lua_pushboolean(L, pressed);
	return 1;
}

static int lua_mouse_is_button_released(lua_State* L)
{
	milk::MouseButtons button = (milk::MouseButtons)luaL_checkinteger(L, 1);
	bool released = milk::mouse_is_button_released(button);
	lua_pushboolean(L, released);
	return 1;
}


static int lua_mouse_get_position(lua_State* L)
{
	// SDL's logical resolution filter only applies to events pumped through the event loop, not the real time state updates.
	// This means that we have to handle it ourselves.
	std::tuple<int, int> pos = milk::mouse_get_position();
	std::tuple<int, int> winsize = milk::window_get_size();
	std::tuple<int, int> resolution = milk::graphics_get_resolution();

	float normMousex = (float)std::get<0>(pos) / std::get<0>(winsize);
	float normMouseY = (float)std::get<1>(pos) / std::get<1>(winsize);
	int mousex = (int)(normMousex * std::get<0>(resolution));
	int mousey = (int)(normMouseY * std::get<1>(resolution));

	lua_pushinteger(L, mousex);
	lua_pushinteger(L, mousey);
	return 2;
}

static int lua_mouse_get_scroll(lua_State* L)
{
	int scroll = milk::mouse_get_scroll();
	lua_pushinteger(L, scroll);
	return 1;
}

static const luaL_Reg lua_mouse_funcs[] = {
	{ "is_button_down", lua_mouse_is_button_down },
	{ "is_button_pressed", lua_mouse_is_button_pressed },
	{ "is_button_released", lua_mouse_is_button_released },
	{ "get_position", lua_mouse_get_position },
	{ "get_scroll", lua_mouse_get_scroll },
	{ nullptr, nullptr }
};

static const milk::luaM_Enum lua_buttons_enum[] = {
	{ "LEFT", (int)milk::MouseButtons::LEFT },
	{ "MIDDLE", (int)milk::MouseButtons::MIDDLE },
	{ "RIGHT", (int)milk::MouseButtons::RIGHT }
};

int luaopen_milk_mouse(lua_State* L)
{
	luaL_newlib(L, lua_mouse_funcs);
	luaM_setenumfield(L, -1, "buttons", lua_buttons_enum, sizeof(lua_buttons_enum));
	return 1;
}
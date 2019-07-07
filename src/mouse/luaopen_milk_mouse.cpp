#include "luaopen_milk_mouse.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "Mouse.h"
#include "Locator.h"
#include "luamlib.h"
#include "graphics/Renderer.h"
#include "window/Window.h"

static int mouse_is_button_down(lua_State* L)
{
	milk::MouseButtons button = (milk::MouseButtons)luaL_checkinteger(L, 1);
	bool down = milk::State::mouse->isButtonDown(button);
	lua_pushboolean(L, down);
	return 1;
}

static int mouse_is_button_pressed(lua_State* L)
{
	milk::MouseButtons button = (milk::MouseButtons)luaL_checkinteger(L, 1);
	bool down = milk::State::mouse->isButtonPressed(button);
	lua_pushboolean(L, down);
	return 1;
}

static int mouse_is_button_released(lua_State* L)
{
	milk::MouseButtons button = (milk::MouseButtons)luaL_checkinteger(L, 1);
	bool down = milk::State::mouse->isButtonReleased(button);
	lua_pushboolean(L, down);
	return 1;
}

static int mouse_get_position(lua_State* L)
{
	// SDL's logical resolution filter only applies to events pumped through the event loop, not the real time state updates.
	// This means that we have to handle it ourselves.
	std::tuple<int, int> pos = milk::State::mouse->getPosition();
	std::tuple<int, int> winsize = milk::State::window->size();
	std::tuple<int, int> resolution = milk::State::renderer->resolution();

	float normMousex = (float)std::get<0>(pos) / std::get<0>(winsize);
	float normMouseY = (float)std::get<1>(pos) / std::get<1>(winsize);
	int mousex = (int)(normMousex * std::get<0>(resolution));
	int mousey = (int)(normMouseY * std::get<1>(resolution));

	lua_pushinteger(L, mousex);
	lua_pushinteger(L, mousey);
	return 2;
}

static int mouse_get_scroll(lua_State* L)
{
	int scroll = milk::State::mouse->getScroll();
	lua_pushinteger(L, scroll);
	return 1;
}

static const luaL_Reg mouse_funcs[] = {
	{ "is_button_down", mouse_is_button_down },
	{ "is_button_pressed", mouse_is_button_pressed },
	{ "is_button_released", mouse_is_button_released },
	{ "get_position", mouse_get_position },
	{ "get_scroll", mouse_get_scroll },
	{ nullptr, nullptr }
};

static const milk::luaM_Enum buttons_enum[] = {
	{ "LEFT", (int)milk::MouseButtons::LEFT },
	{ "MIDDLE", (int)milk::MouseButtons::MIDDLE },
	{ "RIGHT", (int)milk::MouseButtons::RIGHT }
};

int luaopen_milk_mouse(lua_State* L)
{
	luaL_newlib(L, mouse_funcs);
	luaM_setenumfield(L, -1, "buttons", buttons_enum, sizeof(buttons_enum));
	return 1;
}
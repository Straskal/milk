#include "luaopen_milk_mouse.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "Mouse.h"
#include "core/Locator.h"
#include "core/luamlib.h"
#include "graphics/Renderer.h"
#include "window/Window.h"

static int mouse_is_button_down(lua_State* L) {
	int isnum;
	int button = (int)lua_tointegerx(L, 1, &isnum);
	if (isnum) {
		bool down = milk::Locator::mouse->isButtonDown((milk::MouseButtons)button);
		lua_pushboolean(L, down);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

static int mouse_is_button_pressed(lua_State* L) {
	int isnum;
	int button = (int)lua_tointegerx(L, 1, &isnum);
	if (isnum) {
		bool pressed = milk::Locator::mouse->isButtonPressed((milk::MouseButtons)button);
		lua_pushboolean(L, pressed);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

static int mouse_is_button_released(lua_State* L) {
	int isnum;
	int button = (int)lua_tointegerx(L, 1, &isnum);
	if (isnum) {
		bool released = milk::Locator::mouse->isButtonReleased((milk::MouseButtons)button);
		lua_pushboolean(L, released);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

static int mouse_get_win_position(lua_State* L) {
	std::tuple<int, int> pos = milk::Locator::mouse->getPosition();
	lua_pushinteger(L, std::get<0>(pos));
	lua_pushinteger(L, std::get<1>(pos));
	return 2;
}

static int mouse_get_position(lua_State* L) {
	// SDL's logical resolution filter only applies to events pumped through the event loop, not the real time state updates.
	// This means that we have to handle it ourselves.
	std::tuple<int, int> pos = milk::Locator::mouse->getPosition();
	std::tuple<int, int> winsize = milk::Locator::window->size();
	std::tuple<int, int> resolution = milk::Locator::renderer->resolution();

	int mousex = (int)(((float)std::get<0>(pos) / std::get<0>(winsize)) * std::get<0>(resolution));
	int mousey = (int)(((float)std::get<1>(pos) / std::get<1>(winsize)) * std::get<1>(resolution));

	lua_pushinteger(L, mousex);
	lua_pushinteger(L, mousey);
	return 2;
}

static const luaL_Reg mouse_funcs[] = {
	{ "is_button_down", mouse_is_button_down },
	{ "is_button_pressed", mouse_is_button_pressed },
	{ "is_button_released", mouse_is_button_released },
	{ "get_position", mouse_get_position },
	{ "get_win_position", mouse_get_win_position },
	{ NULL, NULL }
};

static const milk::luaM_Enum buttons_enum[] = {
	{ "LEFT", milk::MouseButtons::MOUSE_LEFT },
	{ "MIDDLE", milk::MouseButtons::MOUSE_MIDDLE },
	{ "RIGHT", milk::MouseButtons::MOUSE_RIGHT }
};

int milk::luaopen_milk_mouse(lua_State* L) {
	luaL_newlib(L, mouse_funcs);
	luaM_setenumfield(L, -1, "buttons", buttons_enum, sizeof(buttons_enum) / sizeof(luaM_Enum));
	return 1;
}
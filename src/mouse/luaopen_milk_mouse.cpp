#include "luaopen_milk_mouse.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "Mouse.h"
#include "core/Locator.h"
#include "core/luamlib.h"

static int mouse_is_left_pressed(lua_State* L) {
	bool pressed = milk::Locator::mouse->isLeftButtonPressed();
	lua_pushboolean(L, pressed);
	return 1;
}

static int mouse_is_right_pressed(lua_State* L) {
	bool pressed = milk::Locator::mouse->isRightButtonPressed();
	lua_pushboolean(L, pressed);
	return 1;
}

static int mouse_was_left_pressed(lua_State* L) {
	bool pressed = milk::Locator::mouse->wasLeftButtonPressed();
	lua_pushboolean(L, pressed);
	return 1;
}

static int mouse_was_right_pressed(lua_State* L) {
	bool pressed = milk::Locator::mouse->wasRightButtonPressed();
	lua_pushboolean(L, pressed);
	return 1;
}

static int mouse_was_left_released(lua_State* L) {
	bool released = milk::Locator::mouse->wasLeftButtonReleased();
	lua_pushboolean(L, released);
	return 1;
}

static int mouse_was_right_released(lua_State* L) {
	bool released = milk::Locator::mouse->wasRightButtonReleased();
	lua_pushboolean(L, released);
	return 1;
}

static int mouse_get_position(lua_State* L) {
	std::tuple<int, int> pos = milk::Locator::mouse->getPosition();
	lua_pushboolean(L, std::get<0>(pos));
	lua_pushboolean(L, std::get<1>(pos));
	return 2;
}

static const luaL_Reg mouse_funcs[] = {
	{ "is_left_pressed", mouse_is_left_pressed },
	{ "is_right_pressed", mouse_is_right_pressed },
	{ "was_left_pressed", mouse_was_left_pressed },
	{ "was_right_pressed", mouse_was_right_pressed },
	{ "was_right_released", mouse_was_left_released },
	{ "was_right_released", mouse_was_right_released },
	{ "get_position", mouse_get_position },
	{ NULL, NULL }
};

int milk::luaopen_milk_mouse(lua_State* L) {
	luaL_newlib(L, mouse_funcs);
	return 1;
}
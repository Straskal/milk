#include "luaopen_milk_controller.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "controller.h"
#include "luamlib.h"

static int lua_controller_get_leftx(lua_State* L)
{
	int x = milk::controller_get_leftx();
	lua_pushinteger(L, x);
	return 1;
}

static int lua_controller_get_lefty(lua_State* L)
{
	int y = milk::controller_get_lefty();
	lua_pushinteger(L, y);
	return 1;
}

static int lua_controller_get_rightx(lua_State* L)
{
	int x = milk::controller_get_rightx();
	lua_pushinteger(L, x);
	return 1;
}

static int lua_controller_get_righty(lua_State* L)
{
	int y = milk::controller_get_righty();
	lua_pushinteger(L, y);
	return 1;
}

static int lua_is_button_down(lua_State* L)
{
	SDL_GameControllerButton button = (SDL_GameControllerButton)luaL_checkinteger(L, 1);
	bool down = milk::controller_is_button_down(button);
	lua_pushboolean(L, down);
	return 1;
}

static int lua_is_button_pressed(lua_State* L)
{
	SDL_GameControllerButton button = (SDL_GameControllerButton)luaL_checkinteger(L, 1);
	bool pressed = milk::controller_is_button_pressed(button);
	lua_pushboolean(L, pressed);
	return 1;
}

static int lua_is_button_released(lua_State* L)
{
	SDL_GameControllerButton button = (SDL_GameControllerButton)luaL_checkinteger(L, 1);
	bool released = milk::controller_is_button_released(button);
	lua_pushboolean(L, released);
	return 1;
}

static const luaL_Reg lua_controller_funcs[] = {
	{ "get_leftx", lua_controller_get_leftx },
    { "get_lefty", lua_controller_get_lefty },
    { "get_rightx", lua_controller_get_rightx },
    { "get_righty", lua_controller_get_righty },
	{ "is_button_down", lua_is_button_down },
	{ "is_button_pressed", lua_is_button_pressed },
	{ "is_button_released", lua_is_button_released },
	{ nullptr, nullptr }
};

static const milk::luaM_Enum lua_controller_enum[] = {
	{"A", (int)SDL_CONTROLLER_BUTTON_A},
	{"B", (int)SDL_CONTROLLER_BUTTON_B},
	{"X", (int)SDL_CONTROLLER_BUTTON_X},
	{"Y", (int)SDL_CONTROLLER_BUTTON_Y},
	{"BACK", (int)SDL_CONTROLLER_BUTTON_BACK},
	{"START", (int)SDL_CONTROLLER_BUTTON_START},
	{"SHOULDER_LEFT", (int)SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
	{"SHOULDER_RIGHT", (int)SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},
	{"PAD_UP", (int)SDL_CONTROLLER_BUTTON_DPAD_UP},
	{"PAD_DOWN", (int)SDL_CONTROLLER_BUTTON_DPAD_DOWN},
	{"PAD_LEFT", (int)SDL_CONTROLLER_BUTTON_DPAD_LEFT},
	{"PAD_RIGHT", (int)SDL_CONTROLLER_BUTTON_DPAD_RIGHT}
};

int luaopen_milk_controller(lua_State* L)
{
	luaL_newlib(L, lua_controller_funcs);
	luaM_setenumfield(L, -1, "buttons", lua_controller_enum, sizeof(lua_controller_enum));
	return 1;
}
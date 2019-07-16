#include "luaopen_milk_controller.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "controller.h"
#include "luamlib.h"

static int lua_get_axis_value(lua_State* L)
{
	SDL_GameControllerAxis axis = (SDL_GameControllerAxis)luaL_checkinteger(L, 1);
	float axis_value = milk::controller_get_axis_value(axis);
	lua_pushnumber(L, axis_value);
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
	{"get_axis_value", lua_get_axis_value},
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
	{"GUIDE", (int)SDL_CONTROLLER_BUTTON_GUIDE},
	{"START", (int)SDL_CONTROLLER_BUTTON_START},
	{"LEFT_STICK", (int)SDL_CONTROLLER_BUTTON_LEFTSTICK},
	{"RIGHT_STICK", (int)SDL_CONTROLLER_BUTTON_RIGHTSTICK},
	{"SHOULDER_LEFT", (int)SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
	{"SHOULDER_RIGHT", (int)SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},
	{"PAD_UP", (int)SDL_CONTROLLER_BUTTON_DPAD_UP},
	{"PAD_DOWN", (int)SDL_CONTROLLER_BUTTON_DPAD_DOWN},
	{"PAD_LEFT", (int)SDL_CONTROLLER_BUTTON_DPAD_LEFT},
	{"PAD_RIGHT", (int)SDL_CONTROLLER_BUTTON_DPAD_RIGHT}
};

static const milk::luaM_Enum lua_axes_enum[] = {
	{"LEFT_X", (int)SDL_CONTROLLER_AXIS_LEFTX},
	{"LEFT_Y", (int)SDL_CONTROLLER_AXIS_LEFTY},
	{"RIGHT_X", (int)SDL_CONTROLLER_AXIS_RIGHTX},
	{"RIGHT_Y", (int)SDL_CONTROLLER_AXIS_RIGHTY},
	{"LEFT_TRIGGER", (int)SDL_CONTROLLER_AXIS_TRIGGERLEFT},
	{"RIGHT_TRIGGER", (int)SDL_CONTROLLER_AXIS_TRIGGERRIGHT},
};

int luaopen_milk_controller(lua_State* L)
{
	luaL_newlib(L, lua_controller_funcs);
	luaM_setenumfield(L, -1, "buttons", lua_controller_enum, sizeof(lua_controller_enum));
	luaM_setenumfield(L, -1, "axes", lua_axes_enum, sizeof(lua_axes_enum));
	return 1;
}
#include "luaopen_milk_controller.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "controller.h"
#include "luamlib.h"

static int lua_get_axis_value(lua_State* L)
{
	milk::c_axis axis = (milk::c_axis)luaL_checkinteger(L, 1);
	float axis_value = milk::controller_get_axis_value(axis);
	lua_pushnumber(L, axis_value);
	return 1;
}

static int lua_is_button_down(lua_State* L)
{
	milk::c_button button = (milk::c_button)luaL_checkinteger(L, 1);
	bool down = milk::controller_is_button_down(button);
	lua_pushboolean(L, down);
	return 1;
}

static int lua_is_button_pressed(lua_State* L)
{
	milk::c_button button = (milk::c_button)luaL_checkinteger(L, 1);
	bool pressed = milk::controller_is_button_pressed(button);
	lua_pushboolean(L, pressed);
	return 1;
}

static int lua_is_button_released(lua_State* L)
{
	milk::c_button button = (milk::c_button)luaL_checkinteger(L, 1);
	bool released = milk::controller_is_button_released(button);
	lua_pushboolean(L, released);
	return 1;
}

static int lua_rumble(lua_State* L)
{
	float intensity = luaL_checknumber(L, 1);
	int duration = luaL_checkinteger(L, 2);
	milk::rumble(intensity, duration);
	return 1;
}

static const luaL_Reg lua_controller_funcs[] = {
	{"get_axis_value", lua_get_axis_value},
	{"is_button_down", lua_is_button_down },
	{"is_button_pressed", lua_is_button_pressed},
	{"is_button_released", lua_is_button_released},
	{"rumble", lua_rumble},
	{ nullptr, nullptr }
};

static const milk::luaM_Enum lua_controller_enum[] = {
	{"A", (int)milk::c_button::A},
	{"B", (int)milk::c_button::B},
	{"X", (int)milk::c_button::X},
	{"Y", (int)milk::c_button::Y},
	{"BACK", (int)milk::c_button::BACK},
	{"GUIDE", (int)milk::c_button::GUIDE},
	{"START", (int)milk::c_button::START},
	{"LEFT_STICK", (int)milk::c_button::LEFTSTICK},
	{"RIGHT_STICK", (int)milk::c_button::RIGHTSTICK},
	{"SHOULDER_LEFT", (int)milk::c_button::LEFTSHOULDER},
	{"SHOULDER_RIGHT", (int)milk::c_button::RIGHTSHOULDER},
	{"PAD_UP", (int)milk::c_button::DPAD_UP},
	{"PAD_DOWN", (int)milk::c_button::DPAD_DOWN},
	{"PAD_LEFT", (int)milk::c_button::DPAD_LEFT},
	{"PAD_RIGHT", (int)milk::c_button::DPAD_RIGHT}
};

static const milk::luaM_Enum lua_axes_enum[] = {
	{"LEFT_X", (int)milk::c_axis::LEFTX},
	{"LEFT_Y", (int)milk::c_axis::LEFTY},
	{"RIGHT_X", (int)milk::c_axis::RIGHTX},
	{"RIGHT_Y", (int)milk::c_axis::RIGHTY},
	{"LEFT_TRIGGER", (int)milk::c_axis::TRIGGERLEFT},
	{"RIGHT_TRIGGER", (int)milk::c_axis::TRIGGERRIGHT},
};

int luaopen_milk_controller(lua_State* L)
{
	luaL_newlib(L, lua_controller_funcs);
	luaM_setenumfield(L, -1, "buttons", lua_controller_enum, sizeof(lua_controller_enum));
	luaM_setenumfield(L, -1, "axes", lua_axes_enum, sizeof(lua_axes_enum));
	return 1;
}
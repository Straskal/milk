#include "luaopen_milk.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include "SDL.h"

#include "luamlib.h"
#include "audio/audio.h"
#include "graphics/graphics.h"
#include "window/window.h"
#include "joystick/joystick.h"
#include "controller/controller.h"

static bool initialized = false;

static int milk_init(lua_State* L)
{
	if (initialized)
		return 0;
	if (milk::window_init()
		&& milk::graphics_init(milk::window_get_handle())
		&& milk::audio_init()
		&& milk::joystick_init()
		&& milk::controller_init()) {
		initialized = true;
		return 0;
	}
	return luaL_error(L, "could not start milk!");
}

static int milk_quit(lua_State* L)
{
	(void)L;
	if (initialized) {
		milk::audio_quit();
		milk::graphics_quit();
		milk::window_quit();
		initialized = false;
	}
	return 0;
}

static const luaL_Reg milk_funcs[] = {
	{ "init", milk_init },
	{ "quit", milk_quit },
	{ nullptr, nullptr }
};

int luaopen_milk(lua_State* L)
{
	milk::luaM_openlibs(L);
	luaL_newlib(L, milk_funcs);
	return 1;
}
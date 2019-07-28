#include "luaopen_milk_messenger.h"

extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
}

#include "messenger.h"
#include "luamlib.h"

static int lua_set_filepath(lua_State* L)
{
	const char* new_path = luaL_checkstring(L, 1);
	milk::set_filepath((char*)new_path);
	return 1;
}

static const luaL_Reg lua_messenger_funcs[] = {
	{"set_filepath", lua_set_filepath},
	{ nullptr, nullptr }
};

int luaopen_milk_messenger(lua_State* L)
{
	luaL_newlib(L, lua_messenger_funcs);
	return 1;
}
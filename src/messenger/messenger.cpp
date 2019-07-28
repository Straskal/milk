#include "messenger.h"

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#include <iostream>
#include "SDL.h"

static char* filepath = (char*)"./scripts/listeners.lua";

static const char* listeners_ref[] = {
    "controller_connect_listener",
    "controller_disconnect_listener"
};

void milk::set_filepath(char* new_path)
{
    filepath = new_path;
}

void milk::send_message(int listener)
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    luaL_loadfile(L, filepath);
    lua_pcall(L, 0, 0, 0);
    lua_getglobal(L, listeners_ref[listener]);
    if(lua_pcall(L, 0, 0, 0) != LUA_OK)
        std::cout << "Error calling lua function: " << lua_tostring(L, -1) << std::endl;
}
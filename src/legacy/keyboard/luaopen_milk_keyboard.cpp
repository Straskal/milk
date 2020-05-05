#include "luaopen_milk_keyboard.h"

#include "keyboard.h"
#include "luamlib.h"

static int lua_is_key_down(lua_State* L)
{
	milk::keys key = (milk::keys)luaL_checkinteger(L, 1);
	bool down = milk::keyboard_is_key_down(key);
	lua_pushboolean(L, down);
	return 1;
}

static int lua_is_key_pressed(lua_State* L)
{
	milk::keys key = (milk::keys)luaL_checkinteger(L, 1);
	bool pressed = milk::keyboard_is_key_pressed(key);
	lua_pushboolean(L, pressed);
	return 1;
}

static int lua_is_key_released(lua_State* L)
{
	milk::keys key = (milk::keys)luaL_checkinteger(L, 1);
	bool released = milk::keyboard_is_key_released(key);
	lua_pushboolean(L, released);
	return 1;
}

static const luaL_Reg lua_keyboard_funcs[] = {
	{ "is_key_down", lua_is_key_down },
	{ "is_key_pressed", lua_is_key_pressed },
	{ "is_key_released", lua_is_key_released },
	{ nullptr, nullptr }
};

static const milk::luaM_Enum lua_keys_enum[] = {
	{ "A", (int)milk::keys::A },
	{ "B", (int)milk::keys::B },
	{ "C", (int)milk::keys::C },
	{ "D", (int)milk::keys::D },
	{ "E", (int)milk::keys::E },
	{ "F", (int)milk::keys::F },
	{ "G", (int)milk::keys::G },
	{ "H", (int)milk::keys::H },
	{ "I", (int)milk::keys::I },
	{ "J", (int)milk::keys::J },
	{ "K", (int)milk::keys::K },
	{ "L", (int)milk::keys::L },
	{ "M", (int)milk::keys::M },
	{ "N", (int)milk::keys::N },
	{ "O", (int)milk::keys::O },
	{ "P", (int)milk::keys::P },
	{ "Q", (int)milk::keys::Q },
	{ "R", (int)milk::keys::R },
	{ "S", (int)milk::keys::S },
	{ "T", (int)milk::keys::T },
	{ "U", (int)milk::keys::U },
	{ "V", (int)milk::keys::V },
	{ "W", (int)milk::keys::W },
	{ "X", (int)milk::keys::X },
	{ "Y", (int)milk::keys::Y },
	{ "Z", (int)milk::keys::Z },
	{ "NUM1", (int)milk::keys::NUM1 },
	{ "NUM2", (int)milk::keys::NUM2 },
	{ "NUM3", (int)milk::keys::NUM3 },
	{ "NUM4", (int)milk::keys::NUM4 },
	{ "NUM5", (int)milk::keys::NUM5 },
	{ "NUM6", (int)milk::keys::NUM6 },
	{ "NUM7", (int)milk::keys::NUM7 },
	{ "NUM8", (int)milk::keys::NUM8 },
	{ "NUM9", (int)milk::keys::NUM9 },
	{ "NUM0", (int)milk::keys::NUM0 },
	{ "RETURN", (int)milk::keys::RETURN },
	{ "ESCAPE", (int)milk::keys::ESCAPE },
	{ "BACKSPACE", (int)milk::keys::BACKSPACE },
	{ "TAB", (int)milk::keys::TAB },
	{ "SPACE", (int)milk::keys::SPACE },
	{ "MINUS", (int)milk::keys::MINUS },
	{ "EQUALS", (int)milk::keys::EQUALS },
	{ "LEFTBRACKET", (int)milk::keys::LEFTBRACKET },
	{ "RIGHTBRACKET", (int)milk::keys::RIGHTBRACKET },
	{ "BACKSLASH", (int)milk::keys::BACKSLASH },
	{ "SEMICOLON", (int)milk::keys::SEMICOLON },
	{ "APOSTROPHE", (int)milk::keys::APOSTROPHE },
	{ "TILDE", (int)milk::keys::GRAVE },
	{ "COMMA", (int)milk::keys::COMMA },
	{ "PERIOD", (int)milk::keys::PERIOD },
	{ "SLASH", (int)milk::keys::SLASH },
	{ "CAPSLOCK", (int)milk::keys::CAPSLOCK },
	{ "F1", (int)milk::keys::F1 },
	{ "F2", (int)milk::keys::F2 },
	{ "F3", (int)milk::keys::F3 },
	{ "F4", (int)milk::keys::F4 },
	{ "F5", (int)milk::keys::F5 },
	{ "F6", (int)milk::keys::F6 },
	{ "F7", (int)milk::keys::F7 },
	{ "F8", (int)milk::keys::F8 },
	{ "F9", (int)milk::keys::F9 },
	{ "F10", (int)milk::keys::F10 },
	{ "F11", (int)milk::keys::F11 },
	{ "F12", (int)milk::keys::F12 },
	{ "DELETE", (int)milk::keys::DELETE },
	{ "RIGHT", (int)milk::keys::RIGHT },
	{ "LEFT", (int)milk::keys::LEFT },
	{ "DOWN", (int)milk::keys::DOWN },
	{ "UP", (int)milk::keys::UP },
	{ "LCTRL", (int)milk::keys::LCTRL },
	{ "LSHIFT", (int)milk::keys::LSHIFT },
	{ "LALT", (int)milk::keys::LALT },
	{ "RCTRL", (int)milk::keys::RCTRL },
	{ "RSHIFT", (int)milk::keys::RSHIFT }
};

int luaopen_milk_keyboard(lua_State* L)
{
	luaL_newlib(L, lua_keyboard_funcs);
	luaM_setenumfield(L, -1, "keys", lua_keys_enum, sizeof(lua_keys_enum));
	return 1;
}

#include "luaopen_milk_keyboard.h"

#include "keyboard.h"
#include "luamlib.h"

static int lua_is_key_down(lua_State* L)
{
	milk::Keys key = (milk::Keys)luaL_checkinteger(L, 1);
	bool down = milk::keyboard_is_key_down(key);
	lua_pushboolean(L, down);
	return 1;
}

static int lua_is_key_pressed(lua_State* L)
{
	milk::Keys key = (milk::Keys)luaL_checkinteger(L, 1);
	bool pressed = milk::keyboard_is_key_pressed(key);
	lua_pushboolean(L, pressed);
	return 1;
}

static int lua_is_key_released(lua_State* L)
{
	milk::Keys key = (milk::Keys)luaL_checkinteger(L, 1);
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
	{ "A", (int)milk::Keys::A },
	{ "B", (int)milk::Keys::B },
	{ "C", (int)milk::Keys::C },
	{ "D", (int)milk::Keys::D },
	{ "E", (int)milk::Keys::E },
	{ "F", (int)milk::Keys::F },
	{ "G", (int)milk::Keys::G },
	{ "H", (int)milk::Keys::H },
	{ "I", (int)milk::Keys::I },
	{ "J", (int)milk::Keys::J },
	{ "K", (int)milk::Keys::K },
	{ "L", (int)milk::Keys::L },
	{ "M", (int)milk::Keys::M },
	{ "N", (int)milk::Keys::N },
	{ "O", (int)milk::Keys::O },
	{ "P", (int)milk::Keys::P },
	{ "Q", (int)milk::Keys::Q },
	{ "R", (int)milk::Keys::R },
	{ "S", (int)milk::Keys::S },
	{ "T", (int)milk::Keys::T },
	{ "U", (int)milk::Keys::U },
	{ "V", (int)milk::Keys::V },
	{ "W", (int)milk::Keys::W },
	{ "X", (int)milk::Keys::X },
	{ "Y", (int)milk::Keys::Y },
	{ "Z", (int)milk::Keys::Z },
	{ "NUM1", (int)milk::Keys::NUM1 },
	{ "NUM2", (int)milk::Keys::NUM2 },
	{ "NUM3", (int)milk::Keys::NUM3 },
	{ "NUM4", (int)milk::Keys::NUM4 },
	{ "NUM5", (int)milk::Keys::NUM5 },
	{ "NUM6", (int)milk::Keys::NUM6 },
	{ "NUM7", (int)milk::Keys::NUM7 },
	{ "NUM8", (int)milk::Keys::NUM8 },
	{ "NUM9", (int)milk::Keys::NUM9 },
	{ "NUM0", (int)milk::Keys::NUM0 },
	{ "RETURN", (int)milk::Keys::RETURN },
	{ "ESCAPE", (int)milk::Keys::ESCAPE },
	{ "BACKSPACE", (int)milk::Keys::BACKSPACE },
	{ "TAB", (int)milk::Keys::TAB },
	{ "SPACE", (int)milk::Keys::SPACE },
	{ "MINUS", (int)milk::Keys::MINUS },
	{ "EQUALS", (int)milk::Keys::EQUALS },
	{ "LEFTBRACKET", (int)milk::Keys::LEFTBRACKET },
	{ "RIGHTBRACKET", (int)milk::Keys::RIGHTBRACKET },
	{ "BACKSLASH", (int)milk::Keys::BACKSLASH },
	{ "SEMICOLON", (int)milk::Keys::SEMICOLON },
	{ "APOSTROPHE", (int)milk::Keys::APOSTROPHE },
	{ "TILDE", (int)milk::Keys::GRAVE },
	{ "COMMA", (int)milk::Keys::COMMA },
	{ "PERIOD", (int)milk::Keys::PERIOD },
	{ "SLASH", (int)milk::Keys::SLASH },
	{ "CAPSLOCK", (int)milk::Keys::CAPSLOCK },
	{ "F1", (int)milk::Keys::F1 },
	{ "F2", (int)milk::Keys::F2 },
	{ "F3", (int)milk::Keys::F3 },
	{ "F4", (int)milk::Keys::F4 },
	{ "F5", (int)milk::Keys::F5 },
	{ "F6", (int)milk::Keys::F6 },
	{ "F7", (int)milk::Keys::F7 },
	{ "F8", (int)milk::Keys::F8 },
	{ "F9", (int)milk::Keys::F9 },
	{ "F10", (int)milk::Keys::F10 },
	{ "F11", (int)milk::Keys::F11 },
	{ "F12", (int)milk::Keys::F12 },
	{ "DELETE", (int)milk::Keys::DELETE },
	{ "RIGHT", (int)milk::Keys::RIGHT },
	{ "LEFT", (int)milk::Keys::LEFT },
	{ "DOWN", (int)milk::Keys::DOWN },
	{ "UP", (int)milk::Keys::UP },
	{ "LCTRL", (int)milk::Keys::LCTRL },
	{ "LSHIFT", (int)milk::Keys::LSHIFT },
	{ "LALT", (int)milk::Keys::LALT },
	{ "RCTRL", (int)milk::Keys::RCTRL },
	{ "RSHIFT", (int)milk::Keys::RSHIFT }
};

int luaopen_milk_keyboard(lua_State* L)
{
	luaL_newlib(L, lua_keyboard_funcs);
	luaM_setenumfield(L, -1, "keys", lua_keys_enum, sizeof(lua_keys_enum));
	return 1;
}

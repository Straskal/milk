#include "luaopen_milk_keyboard.h"

#include "core/Locator.h"
#include "core/luamlib.h"

static int is_key_pressed(lua_State* L) {
	int isnum;
	int key = (int)lua_tointegerx(L, 1, &isnum);
	if (isnum) {
		bool down = milk::Locator::keyboard->isKeyPressed((milk::Keys)key);
		lua_pushboolean(L, down);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

static int was_key_pressed(lua_State* L) {
	int isnum;
	int key = (int)lua_tointegerx(L, 1, &isnum);
	if (isnum) {
		bool pressed = milk::Locator::keyboard->wasKeyPressed((milk::Keys)key);
		lua_pushboolean(L, pressed);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

static int was_key_released(lua_State* L) {
	int isnum;
	int key = (int)lua_tointegerx(L, 1, &isnum);
	if (isnum) {
		bool released = milk::Locator::keyboard->wasKeyReleased((milk::Keys)key);
		lua_pushboolean(L, released);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

static const luaL_Reg keyboard_funcs[] = {
	{ "is_key_pressed", is_key_pressed },
	{ "was_key_pressed", was_key_pressed },
	{ "was_key_released", was_key_released },
	{ NULL, NULL }
};

static const milk::luaM_Enum keys_enum[] = {
	{ "A", milk::Keys::A },
	{ "B", milk::Keys::B },
	{ "C", milk::Keys::C },
	{ "D", milk::Keys::D },
	{ "E", milk::Keys::E },
	{ "F", milk::Keys::F },
	{ "G", milk::Keys::G },
	{ "H", milk::Keys::H },
	{ "I", milk::Keys::I },
	{ "J", milk::Keys::J },
	{ "K", milk::Keys::K },
	{ "L", milk::Keys::L },
	{ "M", milk::Keys::M },
	{ "N", milk::Keys::N },
	{ "O", milk::Keys::O },
	{ "P", milk::Keys::P },
	{ "Q", milk::Keys::Q },
	{ "R", milk::Keys::R },
	{ "S", milk::Keys::S },
	{ "T", milk::Keys::T },
	{ "U", milk::Keys::U },
	{ "V", milk::Keys::V },
	{ "W", milk::Keys::W },
	{ "X", milk::Keys::X },
	{ "Y", milk::Keys::Y },
	{ "Z", milk::Keys::Z },
	{ "NUM1", milk::Keys::NUM1 },
	{ "NUM2", milk::Keys::NUM2 },
	{ "NUM3", milk::Keys::NUM3 },
	{ "NUM4", milk::Keys::NUM4 },
	{ "NUM5", milk::Keys::NUM5 },
	{ "NUM6", milk::Keys::NUM6 },
	{ "NUM7", milk::Keys::NUM7 },
	{ "NUM8", milk::Keys::NUM8 },
	{ "NUM9", milk::Keys::NUM9 },
	{ "NUM0", milk::Keys::NUM0 },
	{ "RETURN", milk::Keys::RETURN },
	{ "ESCAPE", milk::Keys::ESCAPE },
	{ "BACKSPACE", milk::Keys::BACKSPACE },
	{ "TAB", milk::Keys::TAB },
	{ "SPACE", milk::Keys::SPACE },
	{ "MINUS", milk::Keys::MINUS },
	{ "EQUALS", milk::Keys::EQUALS },
	{ "LEFTBRACKET", milk::Keys::LEFTBRACKET },
	{ "RIGHTBRACKET", milk::Keys::RIGHTBRACKET },
	{ "BACKSLASH", milk::Keys::BACKSLASH },
	{ "SEMICOLON", milk::Keys::SEMICOLON },
	{ "APOSTROPHE", milk::Keys::APOSTROPHE },
	{ "TILDE", milk::Keys::GRAVE },
	{ "COMMA", milk::Keys::COMMA },
	{ "PERIOD", milk::Keys::PERIOD },
	{ "SLASH", milk::Keys::SLASH },
	{ "CAPSLOCK", milk::Keys::CAPSLOCK },
	{ "F1", milk::Keys::F1 },
	{ "F2", milk::Keys::F2 },
	{ "F3", milk::Keys::F3 },
	{ "F4", milk::Keys::F4 },
	{ "F5", milk::Keys::F5 },
	{ "F6", milk::Keys::F6 },
	{ "F7", milk::Keys::F7 },
	{ "F8", milk::Keys::F8 },
	{ "F9", milk::Keys::F9 },
	{ "F10", milk::Keys::F10 },
	{ "F11", milk::Keys::F11 },
	{ "F12", milk::Keys::F12 },
	{ "DELETE", milk::Keys::DELETE },
	{ "RIGHT", milk::Keys::RIGHT },
	{ "LEFT", milk::Keys::LEFT },
	{ "DOWN", milk::Keys::DOWN },
	{ "UP", milk::Keys::UP },
	{ "LCTRL", milk::Keys::LCTRL },
	{ "LSHIFT", milk::Keys::LSHIFT },
	{ "LALT", milk::Keys::LALT },
	{ "RCTRL", milk::Keys::RCTRL },
	{ "RSHIFT", milk::Keys::RSHIFT }
};

int milk::luaopen_milk_keyboard(lua_State* L) {
	luaL_newlib(L, keyboard_funcs);
	luaM_setenumfield(L, -1, "keys", keys_enum, sizeof(keys_enum)/sizeof(luaM_Enum));
	return 1;
}

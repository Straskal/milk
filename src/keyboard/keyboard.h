#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

namespace milk
{
	// These have a 1:1 relationship with SDL_Scancodes
	enum class Keys
	{
		UNKNOWN,
		A = 4,
		B = 5,
		C = 6,
		D = 7,
		E = 8,
		F = 9,
		G = 10,
		H = 11,
		I = 12,
		J = 13,
		K = 14,
		L = 15,
		M = 16,
		N = 17,
		O = 18,
		P = 19,
		Q = 20,
		R = 21,
		S = 22,
		T = 23,
		U = 24,
		V = 25,
		W = 26,
		X = 27,
		Y = 28,
		Z = 29,
		NUM1 = 30,
		NUM2 = 31,
		NUM3 = 32,
		NUM4 = 33,
		NUM5 = 34,
		NUM6 = 35,
		NUM7 = 36,
		NUM8 = 37,
		NUM9 = 38,
		NUM0 = 39,
		RETURN = 40,
		ESCAPE = 41,
		BACKSPACE = 42,
		TAB = 43,
		SPACE = 44,
		MINUS = 45,
		EQUALS = 46,
		LEFTBRACKET = 47,
		RIGHTBRACKET = 48,
		BACKSLASH = 49,
		SEMICOLON = 51,
		APOSTROPHE = 52,
		GRAVE = 53,
		COMMA = 54,
		PERIOD = 55,
		SLASH = 56,
		CAPSLOCK = 57,
		F1 = 58,
		F2 = 59,
		F3 = 60,
		F4 = 61,
		F5 = 62,
		F6 = 63,
		F7 = 64,
		F8 = 65,
		F9 = 66,
		F10 = 67,
		F11 = 68,
		F12 = 69,
		DELETE = 76,
		RIGHT = 79,
		LEFT = 80,
		DOWN = 81,
		UP = 82,
		LCTRL = 224,
		LSHIFT = 225,
		LALT = 226,
		RCTRL = 228,
		RSHIFT = 229,
		MAX_KEYS
	};

	bool keyboard_update_state();

	bool keyboard_is_key_down(Keys key);
	bool keyboard_is_key_pressed(Keys key);
	bool keyboard_is_key_released(Keys key);
}

#endif
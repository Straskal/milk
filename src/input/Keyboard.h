#ifndef MILK_KEYBOARD_H
#define MILK_KEYBOARD_H

#include "SDL.h"

namespace milk {
	enum class Keys {
		ESCAPE = '\033',
		A = 'a',
		B = 'b',
		C = 'c',
		D = 'd',
		E = 'e',
		F = 'f',
		G = 'g',
		H = 'h',
		I = 'i',
		J = 'j',
		K = 'k',
		L = 'l',
		M = 'm',
		N = 'n',
		O = 'o',
		P = 'p',
		Q = 'q',
		R = 'r',
		S = 's',
		T = 't',
		U = 'u',
		V = 'v',
		W = 'w',
		X = 'x',
		Y = 'y',
		Z = 'z'
	};

	class Keyboard {
	public:
		virtual bool isKeyDown(SDL_Keycode keycode) = 0;
		virtual bool isKeyPressed(SDL_Keycode keycode) = 0;
		virtual bool isKeyReleased(SDL_Keycode keycode) = 0;
	};
}

#endif
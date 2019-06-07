#ifndef MILK_KEYBOARD_H
#define MILK_KEYBOARD_H

#include "SDL.h"

namespace milk {
	class Keyboard {
	public:
		virtual bool isKeyDown(SDL_Keycode keycode) = 0;
		virtual bool isKeyPressed(SDL_Keycode keycode) = 0;
		virtual bool isKeyReleased(SDL_Keycode keycode) = 0;
	};
}

#endif
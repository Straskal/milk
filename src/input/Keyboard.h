#ifndef MILK_KEYBOARD_H
#define MILK_KEYBOARD_H

#include "SDL.h"

namespace milk {
	class Keyboard {
	public:
		static bool getKey(SDL_Keycode keycode);
		static bool getKeyPressed(SDL_Keycode keycode);
		static bool getKeyReleased(SDL_Keycode keycode);
		static void initialize();
		static void updateKeyboardState();

	private:
		static Uint8 previousState_[SDL_NUM_SCANCODES];
		static Uint8 currentState_[SDL_NUM_SCANCODES];
	};
}

#endif

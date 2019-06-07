#ifndef MILK_KEYBOARD_SDL_H
#define MILK_KEYBOARD_SDL_H

#include "input/Keyboard.h"

#include "SDL.h"

namespace milk {
	namespace sdl {
		class Keyboard : public milk::Keyboard {
		public:
			void init();
			void updateState();
			bool isKeyDown(SDL_Keycode keycode) override;
			bool isKeyPressed(SDL_Keycode keycode) override;
			bool isKeyReleased(SDL_Keycode keycode) override;

		private:
			Uint8 m_previousState[SDL_NUM_SCANCODES];
			Uint8 m_currentState[SDL_NUM_SCANCODES];
		};
	}
}

#endif

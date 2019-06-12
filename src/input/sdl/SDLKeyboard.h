#ifndef _SDL_KEYBOARD_H_
#define _SDL_KEYBOARD_H_

#include "input/Keyboard.h"

#include "SDL.h"

namespace milk {
	class SDLKeyboard : public Keyboard {
	public:
		SDLKeyboard();

		void updateState();

		bool isKeyPressed(Keys keycode) override;
		bool wasKeyPressed(Keys keycode) override;
		bool wasKeyReleased(Keys keycode) override;

	private:
		Uint8 m_previousState[SDL_NUM_SCANCODES];
		Uint8 m_currentState[SDL_NUM_SCANCODES];
	};
}

#endif

#ifndef _SDL_KEYBOARD_H_
#define _SDL_KEYBOARD_H_

#include "input/Keyboard.h"

#include "SDL.h"

namespace milk {
	class SDLKeyboard : public Keyboard {
	public:
		SDLKeyboard();

		void updateState();
		bool isKeyDown(Keys keycode) override;
		bool isKeyPressed(Keys keycode) override;
		bool isKeyReleased(Keys keycode) override;

	private:
		Uint8 m_previousState[SDL_NUM_SCANCODES];
		Uint8 m_currentState[SDL_NUM_SCANCODES];
	};
}

#endif

#ifndef _SDL_KEYBOARD_H_
#define _SDL_KEYBOARD_H_

#include "keyboard/Keyboard.h"

#include "SDL.h"

namespace milk
{
	class SDLKeyboard : public Keyboard
	{
	public:
		SDLKeyboard();

		void updateState();

		bool isKeyDown(const Keys keycode) const override;
		bool isKeyPressed(const Keys keycode) const override;
		bool isKeyReleased(const Keys keycode) const override;

	private:
		Uint8 m_previousState[SDL_NUM_SCANCODES];
		Uint8 m_currentState[SDL_NUM_SCANCODES];
	};
}

#endif

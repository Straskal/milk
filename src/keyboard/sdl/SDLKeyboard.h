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

		void tick();

		bool isKeyDown(Keys key) const override;
		bool isKeyPressed(Keys key) const override;
		bool isKeyReleased(Keys key) const override;

	private:
		Uint8 m_previousState[SDL_NUM_SCANCODES];
		Uint8 m_currentState[SDL_NUM_SCANCODES];
	};
}

#endif

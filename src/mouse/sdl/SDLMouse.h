#ifndef _SDL_MOUSE_H_
#define _SDL_MOUSE_H_

#include "SDL.h"

#include "mouse/Mouse.h"

namespace milk {
	class SDLMouse : public Mouse {
	public:
		SDLMouse();

		void updateState();

		std::tuple<int, int> getPosition() const override;
		bool isButtonDown(MouseButtons button) const override;
		bool isButtonPressed(MouseButtons button) const override;
		bool isButtonReleased(MouseButtons button) const override;

	private:
		Uint32 m_currentState;
		Uint32 m_previousState;
		int m_x;
		int m_y;
	};
}

#endif
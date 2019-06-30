#ifndef _SDL_MOUSE_H_
#define _SDL_MOUSE_H_

#include "SDL.h"

#include "mouse/Mouse.h"

namespace milk
{
	class SDLMouse : public Mouse
	{
	public:
		SDLMouse();

		void reset();
		void handleEvent(SDL_Event* e);
		void tick();

		std::tuple<int, int> getPosition() const override;
		bool isButtonDown(const MouseButtons button) const override;
		bool isButtonPressed(const MouseButtons button) const override;
		bool isButtonReleased(const MouseButtons button) const override;
		int getScroll() const override;

	private:
		Uint32 m_currentState;
		Uint32 m_previousState;
		int m_x;
		int m_y;
		int m_scroll;
	};
}

#endif
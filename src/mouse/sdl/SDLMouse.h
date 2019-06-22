#ifndef _SDL_MOUSE_H_
#define _SDL_MOUSE_H_

#include "SDL.h"

#include "mouse/Mouse.h"

namespace milk {
	class SDLMouse : public Mouse {
	public:
		SDLMouse();

		// Called at the beginning of each frame to reset it's scroll value
		void frameBegin();
		void handleEvent(SDL_Event* e);

		// Must be called after polling all events in event loop.
		void updateState();

		std::tuple<int, int> getPosition() const override;
		bool isButtonDown(MouseButtons button) const override;
		bool isButtonPressed(MouseButtons button) const override;
		bool isButtonReleased(MouseButtons button) const override;
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
#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <tuple>

#include "SDL.h"

namespace milk
{
	enum class MouseButtons
	{
		LEFT = 1,
		MIDDLE,
		RIGHT
	};

	void mouse_reset_scroll();
	void mouse_handle_wheel_event(SDL_Event* e);
	void mouse_update_state();

	std::tuple<int, int> mouse_get_position();
	bool mouse_is_button_down(MouseButtons button);
	bool mouse_is_button_pressed(MouseButtons button);
	bool mouse_is_button_released(MouseButtons button);
	int mouse_get_scroll();
}

#endif
#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#include "SDL.h"

namespace milk {
    bool joystick_init();
    void joystick_close();
    void joystick_handle_axis_event(SDL_Event* e);
    int joystick_get_x();
    int joystick_get_y();
}

#endif
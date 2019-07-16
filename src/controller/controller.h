#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "SDL.h"

namespace milk {
    bool controller_init();
    void controller_close();
    void controller_update_state();

    //Axis functions
    float controller_get_axis_value (SDL_GameControllerAxis axis);

    //Button functions
    bool controller_is_button_down(SDL_GameControllerButton button);
    bool controller_is_button_pressed(SDL_GameControllerButton button);
    bool controller_is_button_released(SDL_GameControllerButton button);
}

#endif
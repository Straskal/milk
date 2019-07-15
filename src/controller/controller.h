#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "SDL.h"

namespace milk {
    bool controller_init();
    void controller_close();
    void controller_handle_axis_event();
    //TODO: return normalized axes values;
    int controller_get_leftx();
    int controller_get_lefty();
    int controller_get_rightx();
    int controller_get_righty();
    //ENDTODO
    void controller_update_state();
    bool controller_is_button_down(SDL_GameControllerButton button);
    bool controller_is_button_pressed(SDL_GameControllerButton button);
    bool controller_is_button_released(SDL_GameControllerButton button);
}

#endif
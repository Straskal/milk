#include "controller.h"

#include <iostream>
#include <cstring>
#include <vector>

#include "SDL.h"

static float axes[SDL_CONTROLLER_AXIS_MAX];
static bool current_state[SDL_CONTROLLER_BUTTON_MAX];
static bool previous_state[SDL_CONTROLLER_BUTTON_MAX];
SDL_GameController *controller = NULL;

bool milk::controller_init() 
{
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0)
    {
        std::cout << "SDL_Init: Failed to initialize controller subsystem: " << SDL_GetError() << std::endl;
        return false;
    }

    if (SDL_IsGameController(0))
    {
        controller = SDL_GameControllerOpen(0);
        if (!controller)
        {
            std::cout << "Could not open gamecontroller" << SDL_GetError() << std::endl;
            return false;
        }
    }
    return true;
}

void milk::controller_close()
{
    if (controller)
    {
        SDL_GameControllerClose(controller);
    }
    else
    {
        std::cout << "Could not close gamecontroller" << SDL_GetError() << std::endl;
    }
    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    controller = NULL;
}

float get_axis(SDL_GameControllerAxis axis)
{
    float result = (float)SDL_GameControllerGetAxis(controller, axis) / 32767;

    if (result < -1)
    {
        result = -1.0f;
    }

    return result;
}

void milk::controller_update_state()
{
    for(int i = 0; i < SDL_CONTROLLER_AXIS_MAX; i++)
    {
        axes[i] = get_axis((SDL_GameControllerAxis)i);
    }

    for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
    {
        previous_state[i] = current_state[i];
        current_state[i] = SDL_GameControllerGetButton(controller, (SDL_GameControllerButton)i);
    }
}


float milk::controller_get_axis_value (c_axis axis)
{
    return axes[(int)axis];
}

bool milk::controller_is_button_down(c_button button)
{
	return current_state[(int)button];
}

bool milk::controller_is_button_pressed(c_button button)
{
	return current_state[(int)button] && !previous_state[(int)button];
}

bool milk::controller_is_button_released(c_button button)
{
	return !current_state[(int)button] && previous_state[(int)button];
}
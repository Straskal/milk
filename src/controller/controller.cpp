#include "controller.h"

#include <iostream>
#include <cstring>
#include <vector>

#include "SDL.h"

static float axes[SDL_CONTROLLER_AXIS_MAX];
static bool current_state[SDL_CONTROLLER_BUTTON_MAX];
static bool previous_state[SDL_CONTROLLER_BUTTON_MAX];
static SDL_GameController *controller = NULL;
static SDL_Haptic *haptic = NULL;

static void openHaptic()
{
    if (SDL_Init(SDL_INIT_HAPTIC) < 0)
    {
        std::cout << "SDL_Init: Failed to initialize haptics subsystem" << SDL_GetError() << std::endl;
    }

    haptic = SDL_HapticOpenFromJoystick(SDL_GameControllerGetJoystick(controller));
    if (haptic == NULL) 
    {
        std::cout << "Controller does not support haptics " << SDL_GetError() << std::endl;
    }

    if (SDL_HapticRumbleInit(haptic) != 0)
    {
        std::cout << "Unable to initialize rumble " << SDL_GetError() << std::endl;
        SDL_HapticClose(haptic);
    }
}

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

    openHaptic();
    return true;
}

void milk::controller_close()
{
    if (haptic)
    {
        SDL_HapticClose(haptic);
    }

    if (controller)
    {
        SDL_GameControllerClose(controller);
    }
    else
    {
        std::cout << "Could not close gamecontroller" << SDL_GetError() << std::endl;
    }
    
    SDL_QuitSubSystem(SDL_INIT_HAPTIC);
    haptic = NULL;

    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    controller = NULL;
}

static float get_axis(SDL_GameControllerAxis axis)
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

void milk::rumble(float intensity, int duration)
{
    SDL_HapticRumblePlay(haptic, intensity, duration);
}
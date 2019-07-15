#include "controller.h"

#include <iostream>
#include <cstring>
#include <vector>

#include "SDL.h"

const int CONTROLLER_JOYSTICK_DEAD_ZONE = 8000;
SDL_GameController* controller = NULL;
static int Lx;
static int Ly;
static int Rx;
static int Ry;

bool milk::controller_init() 
{
    if(SDL_Init(SDL_INIT_GAMECONTROLLER) < 0)
    {
        std::cout << "SDL_Init: Failed to initialize gamepad: " << SDL_GetError() << std::endl;
        return false;
    }

    controller = SDL_GameControllerOpen(0);
    return true;
}

void milk::controller_close()
{
    SDL_GameControllerClose(controller);
    controller = NULL;
}

void milk::controller_handle_axis_event()
{
    Lx = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
    Ly = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
    Rx = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
    Ry = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);

    //X axis motion
    //Left of dead zone
    if( Lx < -CONTROLLER_JOYSTICK_DEAD_ZONE )
    {
        Lx = -1;
    }
    //Right of dead zone
    else if( Lx > CONTROLLER_JOYSTICK_DEAD_ZONE )
    {
        Lx =  1;
    }
    else
    {
        Lx = 0;
    }
    //Y axis motion
    //Below of dead zone
    if( Ly < -CONTROLLER_JOYSTICK_DEAD_ZONE )
    {
        Ly = -1;
    }
    //Above of dead zone
    else if( Ly > CONTROLLER_JOYSTICK_DEAD_ZONE )
    {
        Ly =  1;
    }
    else
    {
        Ly = 0;
    }

    //X axis motion
    //Left of dead zone
    if( Rx < -CONTROLLER_JOYSTICK_DEAD_ZONE )
    {
        Rx = -1;
    }
    //Right of dead zone
    else if( Rx > CONTROLLER_JOYSTICK_DEAD_ZONE )
    {
        Rx =  1;
    }
    else
    {
        Rx = 0;
    }
    //Y axis motion
    //Below of dead zone
    if( Ry < -CONTROLLER_JOYSTICK_DEAD_ZONE )
    {
        Ry = -1;
    }
    //Above of dead zone
    else if( Ry > CONTROLLER_JOYSTICK_DEAD_ZONE )
    {
        Ry =  1;
    }
    else
    {
        Ry = 0;
    }
}

int milk::controller_get_leftx()
{
    return Lx;
}

int milk::controller_get_lefty()
{
    return Ly;
}

int milk::controller_get_rightx()
{
    return Rx;
}

int milk::controller_get_righty()
{
    return Ry;
}

static bool current_state[15];
static bool previous_state[15];

void milk::controller_update_state()
{
    for (int i = 0; i < 15; i++)
    {
        previous_state[i] = current_state[i];
        current_state[i] = SDL_GameControllerGetButton(controller, (SDL_GameControllerButton)i);
    }
}

bool milk::controller_is_button_down(SDL_GameControllerButton button)
{
	return current_state[button];
}

bool milk::controller_is_button_pressed(SDL_GameControllerButton button)
{
	return current_state[button] && !previous_state[button];
}

bool milk::controller_is_button_released(SDL_GameControllerButton button)
{
	return !current_state[(int)button] && previous_state[(int)button];
}
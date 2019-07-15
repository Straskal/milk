#include "joystick.h"
#include "SDL.h"

#include<iostream>

const int JOYSTICK_DEAD_ZONE = 8000;
SDL_Joystick* gGameController = NULL;
static int x;
static int y;

bool milk::joystick_init() 
{
    if(SDL_Init(SDL_INIT_JOYSTICK) < 0)
    {
        std::cout << "SDL_Init: Failed to initialize gamepad: " << SDL_GetError() << std::endl;
        return false;
    }

    if(SDL_NumJoysticks() < 1)
    {
        std::cout << "SDL_Joystick: No Joystick connected" << std::endl;
    }
    else
    {
        gGameController = SDL_JoystickOpen(0);
        if (gGameController == NULL)
        {
            std::cout << "SDL_Joystick: Error initializing joystick" << std::endl;
            return false;
        }
    }
    return true;
}

void milk::joystick_close()
{
    SDL_JoystickClose(gGameController);
    gGameController = NULL;
}

void milk::joystick_handle_axis_event(SDL_Event* e)
{
    if(e->jaxis.which == 0)
    {
        //X axis motion
        if( e->jaxis.axis == 0 )
        {
            //Left of dead zone
            if( e->jaxis.value < -JOYSTICK_DEAD_ZONE )
            {
                x = -1;
            }
            //Right of dead zone
            else if( e->jaxis.value > JOYSTICK_DEAD_ZONE )
            {
                x =  1;
            }
            else
            {
                x = 0;
            }
        }
        //Y axis motion
        else if( e->jaxis.axis == 1 )
        {
            //Below of dead zone
            if( e->jaxis.value < -JOYSTICK_DEAD_ZONE )
            {
                y = -1;
            }
            //Above of dead zone
            else if( e->jaxis.value > JOYSTICK_DEAD_ZONE )
            {
                y =  1;
            }
            else
            {
                y = 0;
            }
        }
    }
}

int milk::joystick_get_x()
{
    return x;
}

int milk::joystick_get_y()
{
    return y;
}
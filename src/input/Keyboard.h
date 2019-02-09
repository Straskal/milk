#ifndef MILK_KEYBOARD_H
#define MILK_KEYBOARD_H

#include "SDL.h"

namespace milk
{
    /// Keyboard!!!
    /// TODO: Need to abstract this ish away! This is the SDL implementation.
    class Keyboard
    {
    public:
        Keyboard() = default;

        // Returns true if given key is down.
        static bool getKey(SDL_Keycode keycode);

        // Returns true if given key has been pressed.
        static bool getKeyPressed(SDL_Keycode keycode);

        // Returns true if given key has been released.
        static bool getKeyReleased(SDL_Keycode keycode);

        // Initializes the key states.
        static void initialize();

        // Updates the key states.
        static void updateKeyboardState();

    private:
        static Uint8 previousState_[SDL_NUM_SCANCODES];
        static Uint8 currentState_[SDL_NUM_SCANCODES];
    };
}

#endif

#ifndef _MESSENGER_H_
#define _MESSENGER_H_

#include "SDL.h"

namespace milk
{
      enum class listeners : int
    {
        INVALID = -1,
        CONTROLLER_CONNECT,
        CONTROLLER_DISCONNECT,
        MAX
    };

    void set_filepath (char* filepath);
    void send_message(int listener);
}

#endif
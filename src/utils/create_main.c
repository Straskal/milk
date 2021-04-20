#include "create_main.h"

void create_main_script()
{
    // If the main lua file does not exist, create a template! (Please)
    if (access("main.lua", F_OK) != 0)
    {
        const char *template =
                "-- main.lua\n\n"
                "-- This function is called once when the game starts.\n"
                "function _init()\n\n"
                "end\n\n"
                "-- This function is called once during every frame.\n"
                "function _update()\n\n"
                "end\n\n"
                "-- This function is called once at the end of every frame.\n"
                "function _draw()\n"
                "\tclrs() -- Clear the frame to black before drawing. \n"
                "end\n";

        FILE *file = fopen("main.lua", "w+");
        fputs(template, file);
        fclose(file);
    }
}

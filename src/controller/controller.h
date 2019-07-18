#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

namespace milk {
    enum class c_axis : int
    {
        INVALID = -1,
        LEFTX,
        LEFTY,
        RIGHTX,
        RIGHTY,
        TRIGGERLEFT,
        TRIGGERRIGHT,
        MAX
    };

    enum class c_button : int
    {
        INVALID = -1,
        A,
        B,
        X,
        Y,
        BACK,
        GUIDE,
        START,
        LEFTSTICK,
        RIGHTSTICK,
        LEFTSHOULDER,
        RIGHTSHOULDER,
        DPAD_UP,
        DPAD_DOWN,
        DPAD_LEFT,
        DPAD_RIGHT,
        MAX
    };

    bool controller_init();
    void controller_close();
    void controller_update_state();

    //Axis functions
    float controller_get_axis_value (c_axis axis);

    //Button functions
    bool controller_is_button_down(c_button button);
    bool controller_is_button_pressed(c_button button);
    bool controller_is_button_released(c_button button);

    //Rrrrrrrrrrrrumble
    void rumble(float intensity, int duration);
}

#endif
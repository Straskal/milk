#ifndef __INPUT_H__
#define __INPUT_H__

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
  KEY_0,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_6,
  KEY_7,
  KEY_8,
  KEY_9,
  KEY_A,
  KEY_B,
  KEY_C,
  KEY_D,
  KEY_E,
  KEY_F,
  KEY_G,
  KEY_H,
  KEY_I,
  KEY_J,
  KEY_K,
  KEY_L,
  KEY_M,
  KEY_N,
  KEY_O,
  KEY_P,
  KEY_Q,
  KEY_R,
  KEY_S,
  KEY_T,
  KEY_U,
  KEY_V,
  KEY_W,
  KEY_X,
  KEY_Y,
  KEY_Z,
  KEY_MINUS,
  KEY_EQUALS,
  KEY_LEFTBRACKET,
  KEY_RIGHTBRACKET,
  KEY_BACKSLASH,
  KEY_SEMICOLON,
  KEY_APOSTROPHE,
  KEY_GRAVE,
  KEY_COMMA,
  KEY_PERIOD,
  KEY_SLASH,
  KEY_SPACE,
  KEY_TAB,
  KEY_RETURN,
  KEY_BACKSPACE,
  KEY_DELETE,
  KEY_INSERT,
  KEY_PAGEUP,
  KEY_PAGEDOWN,
  KEY_HOME,
  KEY_END,
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_CAPSLOCK,
  KEY_CTRL,
  KEY_SHIFT,
  KEY_ALT,
  KEY_ESCAPE,
  KEY_MAX,
} Key;

typedef enum
{
  MOUSE_NONE,
  MOUSE_LEFT,
  MOUSE_MIDDLE,
  MOUSE_RIGHT,
} MouseState;

typedef enum
{
  BTN_NONE,
  BTN_START,
  BTN_UP,
  BTN_DOWN,
  BTN_LEFT,
  BTN_RIGHT,
  BTN_A,
  BTN_B,
  BTN_X,
  BTN_Y,
  BTN_MAX,
} ButtonState;

typedef struct
{
  ButtonState state;
  ButtonState previousState;
} Gamepad;

typedef struct
{
  uint8_t state[KEY_MAX];
  uint8_t previousState[KEY_MAX];
} Keyboard;

typedef struct
{
  int x;
  int y;
  int scroll;
  MouseState state;
  MouseState previousState;
} Mouse;

typedef struct
{
  Gamepad gamepad;
  Keyboard keyboard;
  Mouse mouse;
} Input;

void initializeInput(Input *input);
bool isButtonDown(Input *input, ButtonState button);
bool isButtonPressed(Input *input, ButtonState button);
bool isKeyDown(Input *input, Key key);
bool isKeyPressed(Input *input, Key key);
bool isMouseDown(Input *input, MouseState key);
bool isMousePressed(Input *input, MouseState key);

#endif

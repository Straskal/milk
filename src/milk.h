#ifndef __MILK_H__
#define __MILK_H__

#include <stdint.h>

#define MILK_FRAMERATE (1000.0f / 60.0f) /* Fixed 30 FPS */
#define MILK_FRAMEBUF_WIDTH 256
#define MILK_FRAMEBUF_HEIGHT 224
#define MILK_FRAMEBUF_AREA (MILK_FRAMEBUF_WIDTH * MILK_FRAMEBUF_HEIGHT)
#define MILK_WINDOW_WIDTH MILK_FRAMEBUF_WIDTH * 2
#define MILK_WINDOW_HEIGHT MILK_FRAMEBUF_HEIGHT * 2
#define MILK_SPRSHEET_SQRSIZE 256
#define MILK_SPRSHEET_AREA MILK_SPRSHEET_SQRSIZE * MILK_SPRSHEET_SQRSIZE
#define MILK_SPR_SQRSIZE 16 /* Each sprite is considered to be 16x16 px */
#define MILK_SPR_FILENAME "sprsheet.bmp"
#define MILK_FONT_FILENAME "font.bmp"
#define MILK_FONT_WIDTH 128
#define MILK_FONT_HEIGHT 48
#define MILK_FONT_AREA MILK_FONT_WIDTH * MILK_FONT_HEIGHT
#define MILK_CHAR_SQRSIZE 8
#define MILK_FONT_PITCH MILK_FONT_WIDTH * MILK_CHAR_SQRSIZE

#define MILK_BOOL int
#define MILK_TRUE 1
#define MILK_FALSE 0

/*
 * Milk supports 3 channel, 24 bit color pixels.
 */
typedef uint32_t Color32;

#define COLOR_R(c) (c >> 24)
#define COLOR_G(c) (c >> 16)
#define COLOR_B(c) (c >> 8)

/*
 * - The framebuffer is what we actively draw to, and display at the end of each frame.
 * - The sprite sheet is a single image px buffer.
 * - When blitting sprites or fonts, any pixels matching the color key will not be drawn.
 */
typedef struct Video
{
    Color32 framebuffer[MILK_FRAMEBUF_AREA];
    Color32 spritesheet[MILK_SPRSHEET_AREA];
    Color32 font[MILK_FONT_AREA];
    Color32 colorKey;
} Video;

enum
{
	BTN_UP      = 1 << 0,
	BTN_DOWN    = 1 << 1,
	BTN_LEFT    = 1 << 2,
	BTN_RIGHT   = 1 << 3,
	BTN_A       = 1 << 4,
	BTN_B       = 1 << 5,
	BTN_X       = 1 << 6,
	BTN_Y       = 1 << 7
};

typedef struct Gamepad
{
	uint8_t buttonState;
    uint8_t previousButtonState;
} Gamepad;

typedef struct Input
{
	uint32_t mouseX;
	uint32_t mouseY;
	MILK_BOOL mouseDown;
	MILK_BOOL mouseDownPrevious;
	Gamepad gamepad;
} Input;

/*
 * Code is the client side script(s).
 * - state holds the current state of the program. In this case, it is a Lua state.
 */
typedef struct Code
{
	void *state;
} Code;

typedef struct Milk
{
	Input input;
	Video video;
	Code code;
} Milk;

/*
 * Initialize milk, loading all of it's content.
 */
Milk *milkInit();

/*
 * Free milk and all of it's content.
 */
void milkFree(Milk *milk);

/*
 * Update milk's current state.
 */
void milkUpdate(Milk *milk);

/*
 * Returns true if the given button is down.
 */
int milkButton(Input *input, uint8_t button);

/*
 * Draw milk's current state.
 */
void milkDraw(Milk *milk);

/*
 * Clear milk's framebuffer to the specified color.
 */
void milkClear(Video *video, Color32 idx);

/*
 * Set the framebuffer's pixel at the given coordinates.
 */
void milkPixelSet(Video *video, int x, int y, Color32 hex);

/*
 * Draw a solid rectangle to the framebuffer at the given coordinates.
 */
void milkRectFill(Video *video, int x, int y, int w, int h, Color32 hex);

/*
 * Draw a rectangle to the framebuffer at the given coordinates.
 */
void milkRect(Video *video, int x, int y, int w, int h, Color32 hex);

/*
 * Draw a sprite to the framebuffer at the given coordinates.
 *
 * - I'll have a milk sprite with my milk steak please.
 */
void milkSprite(Video *video, int idx, int x, int y);

/*
 * Draw a scaled sprite to the framebuffer at the given coordinates.
 */
void milkScaledSprite(Video *video, int idx, int x, int y, float scale);

/*
 * Draw the text to the framebuffer at the given coordinates.
 */
void milkSpriteFont(Video *video, int x, int y, const char *str);

#endif

#include "milk.h"
#include "milk_bmp.h"
#include "SDL.h"
#include <memory.h>
#include <stdio.h>

#define FIRST_AVAILABLE_RENDERER -1
#define MILK_FRAMEBUF_OUT_BPP 4
#define MILK_FRAMEBUF_PITCH (MILK_FRAMEBUF_WIDTH * MILK_FRAMEBUF_OUT_BPP)

/* HMM: milk should output the appropriate frame buffer. Not a huge deal right now because we don't need this level of control client side. */
static void _flipFramebuffer(uint32_t *frontbuffer, ColorRGB *backbuffer, size_t len)
{
#define PACKED_COLOR(col) (col.r << 24 | (col.g << 16) | (col.b << 8) | 0x00)
	ColorRGB *itr = backbuffer;
	ColorRGB *end = &backbuffer[len - 1];
	while (itr != end)
	{
		ColorRGB col = *(itr++);
		*(frontbuffer++) = PACKED_COLOR(col);
	}
#undef PACKED_COLOR
}

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("Error initializing SDL: %s", SDL_GetError());
		return 0;
	}

	SDL_Window *window = SDL_CreateWindow(
		"milk",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		MILK_WINDOW_WIDTH,
		MILK_WINDOW_HEIGHT,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN_DESKTOP
	);

	if (window == NULL)
	{
		printf("Error creating SDL window: %s", SDL_GetError());
		return 0;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(
		window,
		FIRST_AVAILABLE_RENDERER,
		SDL_RENDERER_ACCELERATED
	);

	if (renderer == NULL)
	{
		printf("Error creating SDL renderer: %s", SDL_GetError());
		return 0;
	}

	SDL_Texture *frontBufferTexture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STATIC,
		MILK_FRAMEBUF_WIDTH,
		MILK_FRAMEBUF_HEIGHT
	);

	uint32_t *frontBufferData = (uint32_t *)calloc(MILK_FRAMEBUF_AREA, sizeof(uint32_t));

	if (frontBufferTexture == NULL)
	{
		printf("Error creating SDL texture: %s", SDL_GetError());
		return 0;
	}

	Milk *milk = milkInit();
	if (milk == NULL)
	{
		printf("Error initializing milk. I know - an error message would have been useful...");
		return 0;
	}

	SDL_RenderSetLogicalSize(renderer, MILK_FRAMEBUF_WIDTH, MILK_FRAMEBUF_HEIGHT);

	int running = MILK_TRUE;
	while (running)
	{
		Uint32 frameStartTicks = SDL_GetTicks();

		milk->input.mouseDownPrevious = milk->input.mouseDown;
		milk->input.mouseDown = 0;
		milk->input.gamepad.previousButtonState = milk->input.gamepad.buttonState;
		milk->input.gamepad.buttonState = 0;

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = MILK_FALSE;
				break;
			case SDL_MOUSEMOTION:
				milk->input.mouseX = event.motion.x;
				milk->input.mouseY = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					milk->input.mouseDown = 1;
				}
				break;
			}
		}

		Gamepad *gamepad = &milk->input.gamepad;
		const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
		if (keyboardState[SDL_SCANCODE_UP]) gamepad->buttonState |= UP;
		if (keyboardState[SDL_SCANCODE_DOWN]) gamepad->buttonState |= DOWN;
		if (keyboardState[SDL_SCANCODE_LEFT]) gamepad->buttonState |= LEFT;
		if (keyboardState[SDL_SCANCODE_RIGHT]) gamepad->buttonState |= RIGHT;
		if (keyboardState[SDL_SCANCODE_Z]) gamepad->buttonState |= A;
		if (keyboardState[SDL_SCANCODE_X]) gamepad->buttonState |= B;
		if (keyboardState[SDL_SCANCODE_C]) gamepad->buttonState |= X;
		if (keyboardState[SDL_SCANCODE_V]) gamepad->buttonState |= Y;

		milkUpdate(milk);
		milkDraw(milk);
		_flipFramebuffer(frontBufferData, milk->video.framebuffer, MILK_FRAMEBUF_AREA);
		SDL_UpdateTexture(frontBufferTexture, NULL, (void *)frontBufferData, MILK_FRAMEBUF_PITCH);
		SDL_RenderCopy(renderer, frontBufferTexture, NULL, NULL);
		SDL_RenderPresent(renderer);

		Uint32 ticks = SDL_GetTicks() - frameStartTicks;
		if (ticks < MILK_FRAMERATE) SDL_Delay(MILK_FRAMERATE - ticks);
	}

	free(frontBufferData);
	milkFree(milk);
	SDL_DestroyTexture(frontBufferTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

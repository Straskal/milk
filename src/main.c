#include "milk.h"
#include "SDL.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("Error initializing SDL: %s", SDL_GetError());
		return 0;
	}

	SDL_Window* window = SDL_CreateWindow("milk", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MILK_WINDOW_WIDTH, MILK_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (window == NULL) 
	{
		printf("Error creating SDL window: %s", SDL_GetError());
		return 0;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("Error creating SDL renderer: %s", SDL_GetError());
		return 0;
	}

	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, MILK_FRAMEBUF_WIDTH, MILK_FRAMEBUF_HEIGHT);
	if (texture == NULL)
	{
		printf("Error creating SDL texture: %s", SDL_GetError());
		return 0;
	}

	MilkMachine* milk = MilkInit();
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
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				running = MILK_FALSE;

			if (event.type == SDL_MOUSEMOTION) 
			{
				milk->input.msx = event.motion.x;
				milk->input.msy = event.motion.y;
			}
		}

		MilkUpdate(milk);
		MilkDraw(milk);

		SDL_UpdateTexture(texture, NULL, (void*)&milk->memory.vram.framebuffer, MILK_FRAMEBUF_PITCH);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		Uint32 ticks = SDL_GetTicks() - frameStartTicks;
		if (ticks < MILK_FRAMERATE) SDL_Delay(MILK_FRAMERATE - ticks);
	}

	MilkFree(milk);

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 1;
}
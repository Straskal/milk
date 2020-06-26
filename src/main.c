/*
 *  MIT License
 *
 *  Copyright(c) 2018 - 2020 Stephen Traskal
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files(the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions :
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "milk.h"
#include "api.h"
#include "console.h"
#include "wav.h"

#include <math.h>
#include <stdio.h>
#include <SDL.h>

#define SDL_FIRST_AVAILABLE_RENDERER -1
#define MILK_FRAMEBUF_PITCH (FRAMEBUFFER_WIDTH * 4)

static SDL_AudioDeviceID gAudioDevice; /* Global audio device so we can access from our methods below. */

 /* Functions to lock and unlock the audio device so you can safely manipulate the milk's audio queue without another thread grabbing for it. */
static void lockAudioDevice()
{
	SDL_LockAudioDevice(gAudioDevice);
}

static void unlockAudioDevice()
{
	SDL_UnlockAudioDevice(gAudioDevice);
}

static void mixCallback(void *userdata, uint8_t *stream, int len)
{
	mixSamplesIntoStream((Audio *)userdata, stream, (size_t)len);
}

static void loadBmp(const char *filename, Color32 *dest, size_t len)
{
	SDL_Surface *bmp = SDL_LoadBMP(filename);
	if (bmp == NULL)
		return;

	uint8_t *bmpPixels = (Uint8 *)bmp->pixels;

	for (size_t i = 0; i < len; i++)
	{
		uint32_t b = *bmpPixels++;
		uint32_t g = *bmpPixels++;
		uint32_t r = *bmpPixels++;

		dest[i] = (r << 16u) | (g << 8u) | (b);
	}

	SDL_FreeSurface(bmp);
}

static void startTextInput()
{
	SDL_StartTextInput();
}

static void stopTextInput()
{
	SDL_StopTextInput();
}

/*
 *******************************************************************************
 * SDL_main
 *******************************************************************************
 */
int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	Milk *milk;
	Console *console;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *frontBufferTexture;
	SDL_AudioDeviceID audioDevice;
	SDL_AudioSpec wantedSpec;
	SDL_AudioSpec actualSpec;

	{
		/*
		 *******************************************************************************
		 * Initialization
		 *******************************************************************************
		 */

		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		{
			printf("Error initializing SDL: %s", SDL_GetError());
			return 1;
		}

		milk = createMilk();
		console = createConsole();
		window = SDL_CreateWindow("milk", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI );
		renderer = SDL_CreateRenderer(window, SDL_FIRST_AVAILABLE_RENDERER, SDL_RENDERER_ACCELERATED);
		frontBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

		SDL_RenderSetLogicalSize(renderer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
	}

	{
		/*
		 *******************************************************************************
		 * Audio device setup
		 *******************************************************************************
		 */

		wantedSpec.freq = AUDIO_FREQUENCY;
		wantedSpec.format = AUDIO_S16LSB;
		wantedSpec.channels = AUDIO_CHANNELS;
		wantedSpec.samples = AUDIO_SAMPLES;
		wantedSpec.callback = mixCallback;
		wantedSpec.userdata = (void *)&milk->audio;

		audioDevice = SDL_OpenAudioDevice(NULL, 0, &wantedSpec, &actualSpec, 0);
		gAudioDevice = audioDevice;

		milk->audio.channels = actualSpec.channels;
		milk->audio.frequency = actualSpec.freq;
		milk->audio.lock = lockAudioDevice;
		milk->audio.unlock = unlockAudioDevice;

		SDL_PauseAudioDevice(audioDevice, 0); /* Pause(0) starts the device. lawl. */
	}

	{
		/*
		 *******************************************************************************
		 * System implementation
		 *******************************************************************************
		 */

		console->input.startTextInput = startTextInput;
        console->input.stopTextInput = stopTextInput;
	}

	{
		/*
		 *******************************************************************************
		 * Load code
		 *******************************************************************************
		 */

		milk->video.loadBMP = loadBmp;

		milkLoadCode(milk);
	}

	while (!milk->shouldQuit)
	{
		Uint32 frameStartTicks = SDL_GetTicks();
		Input *input = &milk->input;
		ConsoleInput *consoleInput = &console->input;

		{
			ButtonState btnState = BTN_NONE;
			ConsoleInputState consoleInputState = CONSOLE_INPUT_NONE;

			input->gamepad.previousButtonState = input->gamepad.buttonState;
			consoleInput->previousState = consoleInput->state;

			/* Poll input events and update input state. */
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
					case SDL_QUIT:
						milk->shouldQuit = true;
						break;
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.sym)
                        {
                            case SDLK_BACKSPACE:
                                consoleInputState |= CONSOLE_INPUT_BACK;
                                break;
                            default:
                                break;
                        }
                        break;
					case SDL_TEXTINPUT:
                        consoleInputState |= CONSOLE_INPUT_CHAR;
						consoleInput->currentChar = event.text.text[0];
						break;
					default:
						break;
				}
			}

			const Uint8 *kbState = SDL_GetKeyboardState(NULL);

			if (kbState[SDL_SCANCODE_P]) btnState |= BTN_START;
			if (kbState[SDL_SCANCODE_UP]) btnState |= BTN_UP;
			if (kbState[SDL_SCANCODE_DOWN]) btnState |= BTN_DOWN;
			if (kbState[SDL_SCANCODE_LEFT]) btnState |= BTN_LEFT;
			if (kbState[SDL_SCANCODE_RIGHT]) btnState |= BTN_RIGHT;
			if (kbState[SDL_SCANCODE_Z]) btnState |= BTN_A;
			if (kbState[SDL_SCANCODE_X]) btnState |= BTN_B;
			if (kbState[SDL_SCANCODE_C]) btnState |= BTN_X;
			if (kbState[SDL_SCANCODE_V]) btnState |= BTN_Y;

            if (kbState[SDL_SCANCODE_RETURN]) consoleInputState |= CONSOLE_INPUT_ENTER;
            if (kbState[SDL_SCANCODE_ESCAPE]) consoleInputState |= CONSOLE_INPUT_ESCAPE;

			input->gamepad.buttonState = btnState;
			consoleInput->state = consoleInputState;
		}

		{
			/* Main loop cycle. */
			updateConsole(console, milk);
			drawConsole(console, milk);
			SDL_UpdateTexture(frontBufferTexture, NULL, (void *)milk->video.framebuffer, MILK_FRAMEBUF_PITCH);
			SDL_RenderCopy(renderer, frontBufferTexture, NULL, NULL);
			SDL_RenderPresent(renderer);
		}

		Uint32 elapsedTicks = SDL_GetTicks() - frameStartTicks;

		if (elapsedTicks < FRAMERATE)
			SDL_Delay((Uint32)(FRAMERATE - elapsedTicks));
	}

	milkUnloadCode(milk);
	SDL_CloseAudioDevice(audioDevice);
	SDL_DestroyTexture(frontBufferTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	freeConsole(console);
	freeMilk(milk);
	SDL_Quit();
	return 0;
}

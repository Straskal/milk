/*
 *  MIT License
 *
 *  Copyright(c) 2018 - 2020 Stephen Traskal
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software andassociated documentation files(the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, andto permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions :
 *
 *  The above copyright notice andthis permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "milk.h"
#include "milkapi.h"
#include "milkcmd.h"

#include <memory.h>
#include <stdio.h>
#include <SDL.h>

#define SDL_FIRST_AVAILABLE_RENDERER -1
#define MILK_FRAMEBUF_PITCH (MILK_FRAMEBUF_WIDTH * 4)
#define MILK_CMD /* We want to compile with the command line. */

static int _gAudioDevice; /* Global audio device so we can access from our methods below. */

 /* Functions to lock and unlock the audio device so you can safely manipulate the milk's audio queue without another thread grabbing for it. */
static void _lockAudioDevice()
{
	SDL_LockAudioDevice(_gAudioDevice);
}

static void _unlockAudioDevice()
{
	SDL_UnlockAudioDevice(_gAudioDevice);
}

/*
 *******************************************************************************
 * System implementation. Need to refactor this. I don't like how it's implemented.
 *******************************************************************************
 */
typedef struct
{
	char textInputChar;
	int hasInput;
	int backspace;
	int enter;
	int escape;
} SystemData;

static SystemData _systemData;

static void _startTextInput()
{
	SDL_StartTextInput();
}

static void _stopTextInput()
{
	SDL_StopTextInput();
}

static int _readInput(char *ch)
{
	*ch = _systemData.textInputChar;
	return _systemData.hasInput;
}

static int _backspace()
{
	return _systemData.backspace;
}

static int _escape()
{
	return _systemData.escape;
}

static int _enter()
{
	return _systemData.enter;
}

/*
 *******************************************************************************
 * SDL_main
 *******************************************************************************
 */
int main(int argc, char *argv[])
{
	Milk *milk;
	MilkCmd *milkCmd;
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

		milk = milkCreate();
		milkCmd = milkCmdCreate();
		window = SDL_CreateWindow("milk", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MILK_WINDOW_WIDTH, MILK_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
		renderer = SDL_CreateRenderer(window, SDL_FIRST_AVAILABLE_RENDERER, SDL_RENDERER_ACCELERATED);
		frontBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, MILK_FRAMEBUF_WIDTH, MILK_FRAMEBUF_HEIGHT);

		SDL_RenderSetLogicalSize(renderer, MILK_FRAMEBUF_WIDTH, MILK_FRAMEBUF_HEIGHT);
		SDL_ShowCursor(0);
	}

	{
		/*
		 *******************************************************************************
		 * Audio device setup
		 *******************************************************************************
		 */

		wantedSpec.freq = MILK_AUDIO_FREQUENCY;
		wantedSpec.format = AUDIO_S16LSB;
		wantedSpec.channels = MILK_AUDIO_CHANNELS;
		wantedSpec.samples = MILK_AUDIO_SAMPLES;
		wantedSpec.callback = milkMixCallback; /* Give the audio spec our mixing callback, and milk's audio as user data. */
		wantedSpec.userdata = (void *)&milk->audio;

		audioDevice = SDL_OpenAudioDevice(NULL, 0, &wantedSpec, &actualSpec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_CHANNELS_CHANGE);
		_gAudioDevice = audioDevice;

		milk->audio.channels = actualSpec.channels;
		milk->audio.frequency = actualSpec.freq;
		milk->audio.lock = _lockAudioDevice;
		milk->audio.unlock = _unlockAudioDevice;

		SDL_PauseAudioDevice(audioDevice, 0); /* Pause(0) starts the device. lawl. */
	}

	{
		/*
		 *******************************************************************************
		 * System implementation
		 *******************************************************************************
		 */

		milk->system.startTextInput = _startTextInput;
		milk->system.stopTextInput = _stopTextInput;
		milk->system.readTextInput = _readInput;
		milk->system.backspace = _backspace;
		milk->system.escape = _escape;
		milk->system.enter = _enter;
	}

	{
		/*
		 *******************************************************************************
		 * Load assets
		 *******************************************************************************
		 */

		milkLoadSpritesheet(&milk->video);
		milkLoadFont(&milk->video);
		milkLoadCode(milk);
	}

	while (!milk->shouldQuit)
	{
		Uint32 frameStartTicks = SDL_GetTicks();
		Input *input = &milk->input;

		{
			/* Reset input state. */
			input->gamepad.previousButtonState = input->gamepad.buttonState;
			input->gamepad.buttonState = 0;

			_systemData.hasInput = 0;
			_systemData.backspace = 0;
			_systemData.enter = 0;
			_systemData.escape = 0;

			/* Poll input events and update input state. */
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
					case SDL_QUIT:
						milkQuit(milk);
						break;
					case SDL_KEYDOWN:
						switch (event.key.keysym.sym)
						{
							case SDLK_BACKSPACE:
								_systemData.backspace = 1;
								break;
							case SDLK_RETURN:
								_systemData.enter = 1;
								break;
							case SDLK_ESCAPE:
								_systemData.escape = 1;
								break;
						}
						break;
					case SDL_TEXTINPUT:
						_systemData.hasInput = 1;
						_systemData.textInputChar = event.text.text[0];
						break;
				}
			}

			const Uint8 *kbState = SDL_GetKeyboardState(NULL);

			if (kbState[SDL_SCANCODE_UP]) input->gamepad.buttonState |= BTN_UP;
			if (kbState[SDL_SCANCODE_DOWN]) input->gamepad.buttonState |= BTN_DOWN;
			if (kbState[SDL_SCANCODE_LEFT]) input->gamepad.buttonState |= BTN_LEFT;
			if (kbState[SDL_SCANCODE_RIGHT]) input->gamepad.buttonState |= BTN_RIGHT;
			if (kbState[SDL_SCANCODE_Z]) input->gamepad.buttonState |= BTN_A;
			if (kbState[SDL_SCANCODE_X]) input->gamepad.buttonState |= BTN_B;
			if (kbState[SDL_SCANCODE_C]) input->gamepad.buttonState |= BTN_X;
			if (kbState[SDL_SCANCODE_V]) input->gamepad.buttonState |= BTN_Y;
		}

		{
			/* Main loop cycle. */
			milkCmdUpdate(milkCmd, milk);
			milkCmdDraw(milkCmd, milk);
			SDL_UpdateTexture(frontBufferTexture, NULL, (void *)milk->video.framebuffer, MILK_FRAMEBUF_PITCH);
			SDL_RenderCopy(renderer, frontBufferTexture, NULL, NULL);
			SDL_RenderPresent(renderer);
		}

		Uint32 elapsedTicks = SDL_GetTicks() - frameStartTicks;

		if (elapsedTicks < MILK_FRAMERATE)
			SDL_Delay((Uint32)(MILK_FRAMERATE - elapsedTicks));
	}

	milkUnloadCode(milk);
	SDL_CloseAudioDevice(audioDevice);
	SDL_DestroyTexture(frontBufferTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	milkCmdFree(milkCmd);
	milkFree(milk);
	SDL_Quit();
	return 0;
}

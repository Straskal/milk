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
#include "api.h"
#include "cmd.h"

#include <math.h>
#include <stdio.h>
#include <SDL.h>

#define SDL_FIRST_AVAILABLE_RENDERER -1
#define MILK_FRAMEBUF_PITCH (FRAMEBUFFER_HEIGHT * 4)

static int gAudioDevice; /* Global audio device so we can access from our methods below. */

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
	mixSamplesIntoStream((Audio *)userdata, stream, len);
}

static void loadWave(Audio *audio, const char *filename, int idx)
{
	SampleData *sampleData = &audio->samples[idx];

	SDL_AudioSpec waveSpec;
	SDL_LoadWAV(filename, &waveSpec, &sampleData->buffer, &sampleData->length);

	SDL_AudioCVT conversion;
	SDL_BuildAudioCVT(&conversion, waveSpec.format, waveSpec.channels, waveSpec.freq, AUDIO_S16LSB, audio->channels, audio->frequency);
	conversion.len = sampleData->length;
	conversion.buf = (Uint8 *)malloc((size_t)conversion.len * conversion.len_mult);
	SDL_memcpy(conversion.buf, sampleData->buffer, sampleData->length);
	SDL_FreeWAV(sampleData->buffer);
	SDL_ConvertAudio(&conversion);

	sampleData->buffer = conversion.buf;
	sampleData->length = (uint32_t)floor(conversion.len * conversion.len_ratio);
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

		milk = createMilk();
		milkCmd = createCmd();
		window = SDL_CreateWindow("milk", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
		renderer = SDL_CreateRenderer(window, SDL_FIRST_AVAILABLE_RENDERER, SDL_RENDERER_ACCELERATED);
		frontBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, FRAMEBUFFER_HEIGHT, FRAMEBUFFER_WIDTH);

		SDL_RenderSetLogicalSize(renderer, FRAMEBUFFER_HEIGHT, FRAMEBUFFER_WIDTH);
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

		audioDevice = SDL_OpenAudioDevice(NULL, 0, &wantedSpec, &actualSpec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_CHANNELS_CHANGE);
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

		milkCmd->input.startTextInput = startTextInput;
		milkCmd->input.stopTextInput = stopTextInput;
	}

	{
		/*
		 *******************************************************************************
		 * Load code
		 *******************************************************************************
		 */

		milk->video.loadBMP = loadBmp;
		milk->audio.loadWAV = loadWave;

		milkLoadCode(milk);
	}

	while (!milk->shouldQuit)
	{
		Uint32 frameStartTicks = SDL_GetTicks();
		Input *input = &milk->input;
		CmdInput *cmdInput = &milkCmd->input;

		{
			ButtonState btnState = 0;
			CmdButtonState cmdBtnState = 0;

			input->gamepad.previousButtonState = input->gamepad.buttonState;

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
								cmdBtnState |= INPUT_BACK;
								break;
							case SDLK_RETURN:
								cmdBtnState |= INPUT_ENTER;
								break;
							case SDLK_ESCAPE:
								cmdBtnState |= INPUT_ESCAPE;
								break;
						}
						break;
					case SDL_TEXTINPUT:
						cmdBtnState |= INPUT_CHAR;
						cmdInput->currentChar = event.text.text[0];
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

			input->gamepad.buttonState = btnState;
			cmdInput->state = cmdBtnState;
		}

		{
			/* Main loop cycle. */
			updateCmd(milkCmd, milk);
			drawCmd(milkCmd, milk);
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
	freeCmd(milkCmd);
	freeMilk(milk);
	SDL_Quit();
	return 0;
}

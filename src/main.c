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
#include "milk_audio.h"
#include "SDL.h"

#include <memory.h>
#include <stdio.h>

#define SDL_FIRST_AVAILABLE_RENDERER -1
#define MILK_FRAMEBUF_PITCH (MILK_FRAMEBUF_WIDTH * 4)

/* Functions to lock and unlock the audio device so you can safely manipulate the milk's audio queue without another thread grabbing for it. */
static int gAudioDevice; /* Global audio device so we can access from our methods below. */
static void _lockAudioDevice();
static void _unlockAudioDevice();

/* The audio stream request callback. This is where mixing happens. */
static void _audioCallback(void *userdata, uint8_t *stream, int len);

int main(int argc, char *argv[])
{
	Milk *milk;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *frontBufferTexture;
	uint32_t *frontBufferData;
	SDL_AudioDeviceID audioDevice;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("Error initializing SDL: %s", SDL_GetError());
		return 0;
	}

	/* We could check for errors here, but we're not asking for much. So it's probably fine until we run into an issue. */
	milk = milkInit();
	window = SDL_CreateWindow("milk", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MILK_WINDOW_WIDTH, MILK_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, SDL_FIRST_AVAILABLE_RENDERER, SDL_RENDERER_ACCELERATED);
	frontBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, MILK_FRAMEBUF_WIDTH, MILK_FRAMEBUF_HEIGHT);
	SDL_RenderSetLogicalSize(renderer, MILK_FRAMEBUF_WIDTH, MILK_FRAMEBUF_HEIGHT);

	SDL_AudioSpec audioSpec;
	audioSpec.freq = MILK_AUDIO_FREQUENCY;
	audioSpec.format = AUDIO_S16LSB;
	audioSpec.channels = MILK_AUDIO_CHANNELS;
	audioSpec.samples = MILK_AUDIO_SAMPLES;

	/* Give the audio spec our mixing callback, and milk's audio as user data. */
	audioSpec.callback = _audioCallback;
	audioSpec.userdata = (void *)&milk->audio;

	audioDevice = SDL_OpenAudioDevice(NULL, 0, &audioSpec, NULL, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_CHANNELS_CHANGE);
	gAudioDevice = audioDevice;
	milk->audio.lock = _lockAudioDevice;
	milk->audio.unlock = _unlockAudioDevice;
	SDL_PauseAudioDevice(audioDevice, 0); /* Pause(0) starts the device. */

	int running = MILK_TRUE;

	while (running)
	{
		Uint32 frameStartTicks = SDL_GetTicks();
		Input *input = &milk->input;

		input->mouseDownPrevious = input->mouseDown;
		input->mouseDown = 0;
		input->gamepad.previousButtonState = input->gamepad.buttonState;
		input->gamepad.buttonState = 0;

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
		if (keyboardState[SDL_SCANCODE_UP]) gamepad->buttonState |= BTN_UP;
		if (keyboardState[SDL_SCANCODE_DOWN]) gamepad->buttonState |= BTN_DOWN;
		if (keyboardState[SDL_SCANCODE_LEFT]) gamepad->buttonState |= BTN_LEFT;
		if (keyboardState[SDL_SCANCODE_RIGHT]) gamepad->buttonState |= BTN_RIGHT;
		if (keyboardState[SDL_SCANCODE_Z]) gamepad->buttonState |= BTN_A;
		if (keyboardState[SDL_SCANCODE_X]) gamepad->buttonState |= BTN_B;
		if (keyboardState[SDL_SCANCODE_C]) gamepad->buttonState |= BTN_X;
		if (keyboardState[SDL_SCANCODE_V]) gamepad->buttonState |= BTN_Y;

		milkUpdate(milk);
		milkDraw(milk);

		SDL_UpdateTexture(frontBufferTexture, NULL, (void *)milk->video.framebuffer, MILK_FRAMEBUF_PITCH);
		SDL_RenderCopy(renderer, frontBufferTexture, NULL, NULL);
		SDL_RenderPresent(renderer);

		Uint32 elapsedTicks = SDL_GetTicks() - frameStartTicks;
		if (elapsedTicks < MILK_FRAMERATE)
			SDL_Delay((Uint32)(MILK_FRAMERATE - elapsedTicks));
	}

	SDL_CloseAudioDevice(audioDevice);
	SDL_DestroyTexture(frontBufferTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	milkFree(milk);
	SDL_Quit();
	return 0;
}

static void _lockAudioDevice()
{
	SDL_LockAudioDevice(gAudioDevice);
}

static void _unlockAudioDevice()
{
	SDL_UnlockAudioDevice(gAudioDevice);
}

/*
 * Mix milk's audio queue samples in the audio device's sample stream.
 * Milk limits the queue size to 16 sounds at a given time, so many mixed sounds should not cause distortion.
 */
static void _audioCallback(void *userdata, uint8_t *stream, int len)
{
	Audio *audio = (Audio *)userdata;
	AudioQueueItem *currentItem = audio->queue;
	AudioQueueItem *previousItem = NULL;
	SDL_memset(stream, 0, len); /* Silence to stream before writing to it. */

	while (currentItem != NULL)
	{
		if (currentItem->remainingLength > 0)
		{
			uint32_t bytesToWrite = ((uint32_t)len > currentItem->remainingLength) ? currentItem->remainingLength : (uint32_t)len;
			double volNormalized = ((double)currentItem->volume / MILK_AUDIO_MAX_VOLUME);

			SDL_MixAudioFormat(stream, currentItem->position, AUDIO_S16LSB, bytesToWrite, (int)round(volNormalized * audio->masterVolume));

			currentItem->position += bytesToWrite;
			currentItem->remainingLength -= bytesToWrite;
			previousItem = currentItem;
			currentItem = currentItem->next;
		}
		else if (currentItem->loop)
		{
			/* Music loops. */
			currentItem->position = currentItem->sampleData->buffer;
			currentItem->remainingLength = currentItem->sampleData->length;
		}
		else
		{
			/* A sound has finished playing and needs to be removed from the queue. */
			if (previousItem == NULL)
				audio->queue = currentItem->next; /* Set root */
			else
				previousItem->next = currentItem->next;

			currentItem->isFree = 1;
			AudioQueueItem *next = currentItem->next;
			currentItem->next = NULL;
			currentItem = next;
		}
	}
}

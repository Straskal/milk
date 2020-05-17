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
#include "milk_bmp.h"
#include "SDL.h"
#include <memory.h>
#include <stdio.h>

#define SDL_FIRST_AVAILABLE_RENDERER -1
#define MILK_FRAMEBUF_PITCH (MILK_FRAMEBUF_WIDTH * 4)

/* Milk outputs the framebuffer as an array of 32 bit colors: 00RRGGBB. Need to shift down the alpha. */
#define SHIFT_ALPHA(c) (c << 8)

/*
 * HACK methods that need to be refactored:
 * - Audio needs to be refactored into a circular queue.
 * - Need to refactor audio queue into a *push API instead of a *callback API. The callback API requires milk to know about locking & unlocking.
 * - The framebuffer should be pulled from milk in the correct format. Need to revisit milk framebuf impl and 24 bit color impl.
 */
static int gAudioDevice;
static void _lockAudioDevice();
static void _unlockAudioDevice();
static void _audioCallback(void *userdata, uint8_t *stream, int len);
static void _flipFramebuffer(uint32_t *frontbuffer, Color32 *backbuffer, size_t len);

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

	{
		milk = milkInit();
		window = SDL_CreateWindow("milk", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MILK_WINDOW_WIDTH, MILK_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
		renderer = SDL_CreateRenderer(window, SDL_FIRST_AVAILABLE_RENDERER, SDL_RENDERER_ACCELERATED);
		frontBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, MILK_FRAMEBUF_WIDTH, MILK_FRAMEBUF_HEIGHT);
		frontBufferData = (uint32_t *)calloc(MILK_FRAMEBUF_WIDTH * MILK_FRAMEBUF_HEIGHT, sizeof(uint32_t));
	}

	{
		SDL_AudioSpec audioSpec;
		audioSpec.freq = MILK_AUDIO_FREQUENCY;
		audioSpec.format = AUDIO_S16LSB;
		audioSpec.channels = MILK_AUDIO_CHANNELS;
		audioSpec.samples = MILK_AUDIO_SAMPLES;
		audioSpec.callback = _audioCallback;
		audioSpec.userdata = (void *)&milk->audio;

		audioDevice = SDL_OpenAudioDevice(NULL, 0, &audioSpec, NULL, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_CHANNELS_CHANGE);
		gAudioDevice = audioDevice;
		milk->audio.lock = _lockAudioDevice;
		milk->audio.unlock = _unlockAudioDevice;
	}

	{
		SDL_RenderSetLogicalSize(renderer, MILK_FRAMEBUF_WIDTH, MILK_FRAMEBUF_HEIGHT);
		SDL_PauseAudioDevice(audioDevice, 0);
		//milkPlayMusic(&milk->audio, 0);
	}

	int running = MILK_TRUE;

	while (running)
	{
		Uint32 frameStartTicks = SDL_GetTicks();

		milk->input.mouseDownPrevious = milk->input.mouseDown;
		milk->input.mouseDown = 0;
		milk->input.gamepad.previousButtonState = milk->input.gamepad.buttonState;
		milk->input.gamepad.buttonState = 0;

		{
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
		}

		{
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
		}

		{
			milkUpdate(milk);
			milkDraw(milk);
		}

		{
			_flipFramebuffer(frontBufferData, milk->video.framebuffer, MILK_FRAMEBUF_WIDTH * MILK_FRAMEBUF_HEIGHT);
			SDL_UpdateTexture(frontBufferTexture, NULL, (void *)frontBufferData, MILK_FRAMEBUF_PITCH);
			SDL_RenderCopy(renderer, frontBufferTexture, NULL, NULL);
			SDL_RenderPresent(renderer);
		}

		Uint32 elapsedTicks = SDL_GetTicks() - frameStartTicks;
		if (elapsedTicks < MILK_FRAMERATE)
			SDL_Delay((Uint32)(MILK_FRAMERATE - elapsedTicks));
	}

	SDL_CloseAudioDevice(audioDevice);
	free(frontBufferData);
	SDL_DestroyTexture(frontBufferTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	milkFree(milk);
	SDL_Quit();
	return 0;
}

/* HMM: milk should output the appropriate frame buffer. Not a huge deal right now because we don't need this level of control client side. */
static void _flipFramebuffer(uint32_t *frontbuffer, Color32 *backbuffer, size_t len)
{
	Color32 *itr = backbuffer;
	Color32 *end = &backbuffer[len - 1];

	while (itr != end)
		*(frontbuffer++) = (uint32_t)SHIFT_ALPHA(*(itr++));
}

static void _lockAudioDevice()
{
	SDL_LockAudioDevice(gAudioDevice);
}

static void _unlockAudioDevice()
{
	SDL_UnlockAudioDevice(gAudioDevice);
}

static void _audioCallback(void *userdata, uint8_t *stream, int len)
{
	Audio *audio = (Audio *)userdata;
	AudioQueueItem *queueItr = audio->queue;
	AudioQueueItem *previousItem = audio->queue;
	int currentLength;
	uint8_t isFadingOut = 0;

	SDL_memset(stream, 0, len);

	while (queueItr != NULL)
	{
		if (queueItr->remainingLength > 0)
		{
			/* If music is marked to fade out */
			if (queueItr->isFading && queueItr->isMusic)
			{
				/* We've found music and we're fading it out. */
				isFadingOut = 1;

				if (queueItr->volume > 0)
					queueItr->volume--;
				else
					queueItr->remainingLength = 0;
			}

			/* If we find another music queued but another one is currently fading out, then don't mix it. */
			if (isFadingOut && queueItr->isMusic && !queueItr->isFading)
				currentLength = 0;
			else
				currentLength = ((uint32_t)len > queueItr->remainingLength) ? queueItr->remainingLength : (uint32_t)len;

			SDL_MixAudioFormat(stream, queueItr->position, AUDIO_S16LSB, currentLength, queueItr->volume);

			queueItr->position += currentLength;
			queueItr->remainingLength -= currentLength;

			previousItem = queueItr;
			queueItr = queueItr->next;
		}
		/* If the sample is music with no remaining length, then start it from the beginning. */
		else if (queueItr->isMusic && !queueItr->isFading)
		{
			queueItr->position = queueItr->sampleData->buffer;
			queueItr->remainingLength = queueItr->sampleData->length;
		}
		/* Else if it's just a sound that happened to end, then remove it from the queue and free the instance. */
		else
		{
			previousItem->next = queueItr->next;
			queueItr->next = NULL;
			queueItr->isFree = 1;
			queueItr = previousItem->next;
		}
	}
}

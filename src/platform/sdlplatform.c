#include <SDL.h>

#include "common.h"
#include "milk.h"

#define SDL_FIRST_AVAILABLE_RENDERER -1
#define FRAMEBUFFER_PITCH (FRAMEBUFFER_WIDTH * sizeof(uint32_t))
#define WINDOW_WIDTH (FRAMEBUFFER_WIDTH * 3)
#define WINDOW_HEIGHT (FRAMEBUFFER_HEIGHT * 3)

static enum flags {
  NONE = 0,
  RUNNING = 1 << 0,
  FULLSCREEN = 1 << 1
} flags = NONE;

static Milk *milk;
static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *frontBufferTexture;
static SDL_AudioDeviceID audioDevice;

static void __freeModules()
{
  SDL_CloseAudioDevice(audioDevice);
  SDL_DestroyTexture(frontBufferTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  freeMilk(milk);
  SDL_Quit();
}

void platform_close()
{
  UNSET_BIT(flags, RUNNING);
}

void platform_lockAudioDevice()
{
  SDL_LockAudioDevice(audioDevice);
}

void platform_unlockAudioDevice()
{
  SDL_UnlockAudioDevice(audioDevice);
}

void platform_startTextInput()
{
  SDL_StartTextInput();
}

void platform_stopTextInput()
{
  SDL_StopTextInput();
}

void platform_toggleFullscreen()
{
  TOGGLE_BIT(flags, FULLSCREEN);
  SDL_SetWindowFullscreen(window, CHECK_BIT(flags, FULLSCREEN) ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

static void __mixCallback(void *userData, uint8_t *stream, int numBytes)
{
  memset(stream, 0, (size_t)numBytes);
  mixSamplesIntoStream((Audio *)userData, (int16_t *)stream, (int)(numBytes / sizeof(int16_t)));
}

int main(int argc, char *argv[])
{
  UNUSED(argc);
  UNUSED(argv);
  atexit(__freeModules);
  SET_BIT(flags, RUNNING);

  // Module initialization
  {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
      printf("Error initializing SDL: %s", SDL_GetError());
      exit(1);
    }
    milk = createMilk();
    window = SDL_CreateWindow("milk",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH, WINDOW_HEIGHT,
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );
    renderer = SDL_CreateRenderer(window,
      SDL_FIRST_AVAILABLE_RENDERER, SDL_RENDERER_ACCELERATED
    );
    frontBufferTexture = SDL_CreateTexture(renderer,
      SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
      FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT
    );
    SDL_RenderSetLogicalSize(renderer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
  }

  // Audio device setup
  {
    SDL_AudioSpec wantedSpec;
    SDL_AudioSpec actualSpec;
    wantedSpec.freq = AUDIO_FREQUENCY;
    wantedSpec.format = AUDIO_S16LSB;
    wantedSpec.channels = AUDIO_OUTPUT_CHANNELS;
    wantedSpec.samples = 4096;
    wantedSpec.callback = __mixCallback;
    wantedSpec.userdata = (void *)&milk->modules.audio;
    audioDevice = SDL_OpenAudioDevice(NULL, 0, &wantedSpec, &actualSpec, 0);
    if (wantedSpec.format != actualSpec.format || wantedSpec.channels != actualSpec.channels
    || wantedSpec.freq != actualSpec.freq || wantedSpec.samples != actualSpec.samples)
    {
      printf("Audio device is not supported.");
      exit(1);
    }
    SDL_PauseAudioDevice(audioDevice, 0);
  }

  initializeMilk(milk);

  const Uint64 deltaTime = SDL_GetPerformanceFrequency() / FRAMERATE;
  Uint64 accumulator = 0;

  while (CHECK_BIT(flags, RUNNING))
  {
    accumulator += deltaTime;

    // Poll input events
    {
      ButtonState btnState = BTN_NONE;
      ExtendedInputState extState = INPUT_NONE;
      char inChar = 0;

      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        switch (event.type)
        {
          case SDL_QUIT:
            platform_close();
            break;
          case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
              case SDLK_BACKSPACE:
                extState |= INPUT_BACK;
                break;
              default:
                break;
            }
            break;
          case SDL_TEXTINPUT:
            extState |= INPUT_CHAR;
            inChar = event.text.text[0];
            break;
          default:
            break;
        }
      }
      const Uint8 *kbState = SDL_GetKeyboardState(NULL);

      if (kbState[SDL_SCANCODE_P])
        btnState |= BTN_START;
      if (kbState[SDL_SCANCODE_UP])
        btnState |= BTN_UP;
      if (kbState[SDL_SCANCODE_DOWN])
        btnState |= BTN_DOWN;
      if (kbState[SDL_SCANCODE_LEFT])
        btnState |= BTN_LEFT;
      if (kbState[SDL_SCANCODE_RIGHT])
        btnState |= BTN_RIGHT;
      if (kbState[SDL_SCANCODE_Z])
        btnState |= BTN_A;
      if (kbState[SDL_SCANCODE_X])
        btnState |= BTN_B;
      if (kbState[SDL_SCANCODE_C])
        btnState |= BTN_X;
      if (kbState[SDL_SCANCODE_V])
        btnState |= BTN_Y;
      if (kbState[SDL_SCANCODE_RETURN])
        extState |= INPUT_ENTER;
      if (kbState[SDL_SCANCODE_ESCAPE])
        extState |= INPUT_ESCAPE;

      updateButtonState(&milk->modules.input, btnState);
      updateExtendedInputState(&milk->modules.input, extState, inChar);
    }

    // Update and draw
    {
      updateMilk(milk);
      drawMilk(milk);
    }

    // Flip the framebuffer
    {
      int pitch;
      uint32_t *frontBuffer = NULL;
      SDL_LockTexture(frontBufferTexture, NULL, (void **)&frontBuffer, &pitch);
      flipFramebuffer(&milk->modules.video, frontBuffer);
      SDL_UnlockTexture(frontBufferTexture);
      SDL_RenderCopy(renderer, frontBufferTexture, NULL, NULL);
      SDL_RenderPresent(renderer);
    }

    // Moderate framerate
    Sint64 delay = accumulator - SDL_GetPerformanceCounter();
    if (delay < 0)
      accumulator -= delay;
    else
      SDL_Delay((Uint32)(delay * 1000 / SDL_GetPerformanceFrequency()));
  }
  return 0;
}

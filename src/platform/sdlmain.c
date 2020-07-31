#include <SDL.h>

#include "common.h"
#include "console.h"
#include "milk.h"

#define SDL_FIRST_AVAILABLE_RENDERER -1
#define FRAMEBUFFER_PITCH (FRAMEBUFFER_WIDTH * sizeof(uint32_t))
#define WINDOW_WIDTH (FRAMEBUFFER_WIDTH * 3)
#define WINDOW_HEIGHT (FRAMEBUFFER_HEIGHT * 3)

static Milk *milk;
static Console *console;
static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *frontBufferTexture;
static SDL_AudioDeviceID audioDevice;

static void __lockAudioDevice() {
  SDL_LockAudioDevice(audioDevice);
}

static void __unlockAudioDevice() {
  SDL_UnlockAudioDevice(audioDevice);
}

static void __startTextInput() {
  SDL_StartTextInput();
}

static void __stopTextInput() {
  SDL_StopTextInput();
}

static Platform platform = {
    .lockAudioDevice = __lockAudioDevice,
    .unlockAudioDevice = __unlockAudioDevice,
    .startTextInput = __startTextInput,
    .stopTextInput = __stopTextInput,
    .flags = 0
};

Platform *getPlatform() {
  return &platform;
}

static void __freeModules() {
  unloadCode(milk);
  SDL_CloseAudioDevice(audioDevice);
  SDL_DestroyTexture(frontBufferTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  freeConsole(console);
  freeMilk(milk);
  SDL_Quit();
}

static void __mixCallback(void *userData, uint8_t *stream, int numBytes) {
  // SDL doesn't guarantee that the output buffer is silent.
  memset(stream, 0, (size_t)numBytes);
  mixSamplesIntoStream((Audio *) userData, (int16_t *) stream, (int) (numBytes / sizeof(int16_t)));
}

static void __pollInput() {
  ButtonState btnState = BTN_NONE;

#ifdef BUILD_WITH_CONSOLE
  ConsoleInputState consoleInputState = CONSOLE_INPUT_NONE;
#endif

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        SET_BIT(platform.flags, PLATFORM_QUIT);
        break;
#ifdef BUILD_WITH_CONSOLE
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_BACKSPACE:
            consoleInputState |= CONSOLE_INPUT_BACK;
            break;
          default: break;
        }
        break;
      case SDL_TEXTINPUT:
        consoleInputState |= CONSOLE_INPUT_CHAR;
        console->input.currentChar = event.text.text[0];
        break;
#endif
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

#ifdef BUILD_WITH_CONSOLE
  if (kbState[SDL_SCANCODE_RETURN]) consoleInputState |= CONSOLE_INPUT_ENTER;
  if (kbState[SDL_SCANCODE_ESCAPE]) consoleInputState |= CONSOLE_INPUT_ESCAPE;
  console->input.previousState = console->input.state;
  console->input.state = consoleInputState;
#endif

  updateButtonState(&milk->input, btnState);
}

static void __frame() {
#ifdef BUILD_WITH_CONSOLE
  updateConsole(console, milk);
  drawConsole(console, milk);
#else
  invokeUpdate(&milk->code);
  invokeDraw(&milk->code);
#endif
}

static void __flipFramebuffer() {
  SDL_UpdateTexture(frontBufferTexture, NULL, (void *) milk->video.framebuffer, FRAMEBUFFER_PITCH);
  SDL_RenderCopy(renderer, frontBufferTexture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
  UNUSED(argc);
  UNUSED(argv);
  atexit(__freeModules);
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    printf("Error initializing SDL: %s", SDL_GetError());
    exit(1);
  }
  milk = createMilk();
  console = createConsole();
  window = SDL_CreateWindow("milk", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,
                            SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  renderer = SDL_CreateRenderer(window, SDL_FIRST_AVAILABLE_RENDERER, SDL_RENDERER_ACCELERATED);
  frontBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
                                         FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
  SDL_RenderSetLogicalSize(renderer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
  SDL_AudioSpec wantedSpec;
  SDL_AudioSpec actualSpec;
  wantedSpec.freq = AUDIO_FREQUENCY;
  wantedSpec.format = AUDIO_S16LSB;
  wantedSpec.channels = AUDIO_OUTPUT_CHANNELS;
  wantedSpec.samples = 4096;
  wantedSpec.callback = __mixCallback;
  wantedSpec.userdata = (void *) &milk->audio;
  audioDevice = SDL_OpenAudioDevice(NULL, 0, &wantedSpec, &actualSpec, 0);
  if (wantedSpec.format != actualSpec.format || wantedSpec.channels != actualSpec.channels
      || wantedSpec.freq != actualSpec.freq || wantedSpec.samples != actualSpec.samples) {
    printf("Audio device is not supported.");
    exit(1);
  }
  const int noDontPauseIt_PlayItInstead = 0;
  SDL_PauseAudioDevice(audioDevice, noDontPauseIt_PlayItInstead);

#ifndef BUILD_WITH_CONSOLE
  loadCode(milk);
  invokeInit(&milk->code);
#endif

  const Uint64 deltaTime = SDL_GetPerformanceFrequency() / FRAMERATE;
  Uint64 accumulator = 0;
  while (!IS_BIT_SET(platform.flags, PLATFORM_QUIT)) {
    accumulator += deltaTime;
    __pollInput();
    __frame();
    __flipFramebuffer();
    Sint64 delay = accumulator - SDL_GetPerformanceCounter();
    if (delay < 0) accumulator -= delay;
    else SDL_Delay((Uint32)(delay * 1000 / SDL_GetPerformanceFrequency()));
  }
  return 0;
}

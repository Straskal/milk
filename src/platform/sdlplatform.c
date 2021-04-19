#include <SDL.h>

#include "common.h"
#include "milk.h"

#define SDL_FIRST_AVAILABLE_RENDERER -1
#define FRAMEBUFFER_PITCH (FRAMEBUFFER_WIDTH * sizeof(uint32_t))
#define WINDOW_WIDTH (FRAMEBUFFER_WIDTH * 3)
#define WINDOW_HEIGHT (FRAMEBUFFER_HEIGHT * 3)

static enum {
  NONE = 0,
  RUNNING = 1 << 0,
  FULLSCREEN = 1 << 1,
} flags = NONE;

static Uint8 sdlKeys[] =
    {
        SDL_SCANCODE_0,
        SDL_SCANCODE_1,
        SDL_SCANCODE_2,
        SDL_SCANCODE_3,
        SDL_SCANCODE_4,
        SDL_SCANCODE_5,
        SDL_SCANCODE_6,
        SDL_SCANCODE_7,
        SDL_SCANCODE_8,
        SDL_SCANCODE_9,
        SDL_SCANCODE_A,
        SDL_SCANCODE_B,
        SDL_SCANCODE_C,
        SDL_SCANCODE_D,
        SDL_SCANCODE_E,
        SDL_SCANCODE_F,
        SDL_SCANCODE_G,
        SDL_SCANCODE_H,
        SDL_SCANCODE_I,
        SDL_SCANCODE_J,
        SDL_SCANCODE_K,
        SDL_SCANCODE_L,
        SDL_SCANCODE_M,
        SDL_SCANCODE_N,
        SDL_SCANCODE_O,
        SDL_SCANCODE_P,
        SDL_SCANCODE_Q,
        SDL_SCANCODE_R,
        SDL_SCANCODE_S,
        SDL_SCANCODE_T,
        SDL_SCANCODE_U,
        SDL_SCANCODE_V,
        SDL_SCANCODE_W,
        SDL_SCANCODE_X,
        SDL_SCANCODE_Y,
        SDL_SCANCODE_Z,
        SDL_SCANCODE_MINUS,
        SDL_SCANCODE_EQUALS,
        SDL_SCANCODE_LEFTBRACKET,
        SDL_SCANCODE_RIGHTBRACKET,
        SDL_SCANCODE_BACKSLASH,
        SDL_SCANCODE_SEMICOLON,
        SDL_SCANCODE_APOSTROPHE,
        SDL_SCANCODE_GRAVE,
        SDL_SCANCODE_COMMA,
        SDL_SCANCODE_PERIOD,
        SDL_SCANCODE_SLASH,
        SDL_SCANCODE_SPACE,
        SDL_SCANCODE_TAB,
        SDL_SCANCODE_RETURN,
        SDL_SCANCODE_BACKSPACE,
        SDL_SCANCODE_DELETE,
        SDL_SCANCODE_INSERT,
        SDL_SCANCODE_PAGEUP,
        SDL_SCANCODE_PAGEDOWN,
        SDL_SCANCODE_HOME,
        SDL_SCANCODE_END,
        SDL_SCANCODE_UP,
        SDL_SCANCODE_DOWN,
        SDL_SCANCODE_LEFT,
        SDL_SCANCODE_RIGHT,
        SDL_SCANCODE_CAPSLOCK,
        SDL_SCANCODE_LCTRL,
        SDL_SCANCODE_LSHIFT,
        SDL_SCANCODE_LALT,
        SDL_SCANCODE_ESCAPE,
};

static SDL_Keycode sdlButtons[] =
    {
        0,
        SDL_SCANCODE_P,
        SDL_SCANCODE_UP,
        SDL_SCANCODE_DOWN,
        SDL_SCANCODE_LEFT,
        SDL_SCANCODE_RIGHT,
        SDL_SCANCODE_Z,
        SDL_SCANCODE_X,
        SDL_SCANCODE_C,
        SDL_SCANCODE_V};

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

static bool backspace = false;
static bool hasCharInput = false;
static char charInput = 0;

bool platform_getCharInput(char *c)
{
  *c = charInput;
  return hasCharInput;
}

bool platform_backspace()
{
  return backspace;
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

static void __initModules()
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
  {
    printf("Error initializing SDL: %s", SDL_GetError());
    exit(1);
  }

  milk = createMilk();

  window = SDL_CreateWindow(
      "milk",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH, WINDOW_HEIGHT,
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

  renderer = SDL_CreateRenderer(
      window,
      SDL_FIRST_AVAILABLE_RENDERER, SDL_RENDERER_ACCELERATED);

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
  SDL_RenderSetLogicalSize(renderer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

  frontBufferTexture = SDL_CreateTexture(
      renderer,
      SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
      FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
}

static void __initAudioDevice()
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

  if (wantedSpec.format != actualSpec.format || wantedSpec.channels != actualSpec.channels || wantedSpec.freq != actualSpec.freq || wantedSpec.samples != actualSpec.samples)
  {
    printf("Audio device is not supported.");
    exit(1);
  }

  SDL_PauseAudioDevice(audioDevice, 0);
}

static void __pollInput()
{
  Input *input = &milk->modules.input;

  memcpy(input->keyboard.previousState, input->keyboard.state, sizeof(input->keyboard.state));
  input->mouse.previousState = input->mouse.state;
  input->mouse.state = 0;
  input->mouse.scroll = 0;
  input->gamepad.previousState = input->gamepad.state;
  input->gamepad.state = 0;

  hasCharInput = false;
  backspace = false;

  SDL_Event event;

  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      platform_close();
      break;
    case SDL_TEXTINPUT:
      charInput = event.text.text[0];
      hasCharInput = true;
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym)
      {
      case SDLK_BACKSPACE:
        backspace = true;
        break;
      }
      break;
    case SDL_MOUSEMOTION:
      input->mouse.x = event.motion.x;
      input->mouse.y = event.motion.y;
      break;
    case SDL_MOUSEWHEEL:
      input->mouse.scroll = event.wheel.y;
      break;
    default:
      break;
    }
  }

  const Uint8 *keys = SDL_GetKeyboardState(NULL);

  for (unsigned i = 0; i < KEY_MAX; i++)
  {
    milk->modules.input.keyboard.state[i] = keys[sdlKeys[i]];
  }

  for (unsigned i = 0; i < BTN_MAX - 1; i++)
  {
    if (keys[sdlButtons[i]])
      milk->modules.input.gamepad.state |= (1 << i);
  }

  Uint32 mouse = SDL_GetMouseState(NULL, NULL);

  if (mouse & SDL_BUTTON(SDL_BUTTON_LEFT))
    input->mouse.state |= MOUSE_LEFT;
  if (mouse & SDL_BUTTON(SDL_BUTTON_MIDDLE))
    input->mouse.state |= MOUSE_MIDDLE;
  if (mouse & SDL_BUTTON(SDL_BUTTON_RIGHT))
    input->mouse.state |= MOUSE_RIGHT;
}

int main(int argc, char *argv[])
{
  UNUSED(argc);
  UNUSED(argv);
  SET_BIT(flags, RUNNING);

  atexit(__freeModules);

  __initModules();
  __initAudioDevice();

  initializeMilk(milk);

  const Uint64 deltaTime = SDL_GetPerformanceFrequency() / FRAMERATE;
  Uint64 accumulator = 0;

  while (CHECK_BIT(flags, RUNNING))
  {
    accumulator += deltaTime;

    __pollInput();

    updateMilk(milk);
    drawMilk(milk);

    int pitch;
    uint32_t *frontBuffer = NULL;
    Video *video = &milk->modules.video;

    SDL_LockTexture(frontBufferTexture, NULL, (void **)&frontBuffer, &pitch);
    memcpy(frontBuffer, video->framebuffer, sizeof(video->framebuffer));
    SDL_UnlockTexture(frontBufferTexture);
    SDL_RenderCopy(renderer, frontBufferTexture, NULL, NULL);
    SDL_RenderPresent(renderer);

    Sint64 delay = accumulator - SDL_GetPerformanceCounter();

    if (delay < 0)
      accumulator -= delay;
    else
      SDL_Delay((Uint32)(delay * 1000 / SDL_GetPerformanceFrequency()));
  }
  return 0;
}

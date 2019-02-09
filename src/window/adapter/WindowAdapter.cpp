#include "WindowAdapter.h"

#include <iostream>

#include "SDL.h"

#include "RendererAdapter.h"

milk::adapter::WindowAdapter::WindowAdapter()
        : title_(""),
          width_(0),
          height_(0),
          fullscreen_(false),
          sdlWindow_(nullptr),
          rendererAdapter_(nullptr)
{
}

milk::adapter::WindowAdapter::~WindowAdapter() = default;

bool milk::adapter::WindowAdapter::init(const std::string& title,
                                        unsigned int width,
                                        unsigned int height,
                                        unsigned int resolutionWidth,
                                        unsigned int resolutionHeight,
                                        bool fullscreen)
{
    title_ = title;
    width_ = width;
    height_ = height;
    fullscreen_ = fullscreen;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_DisplayMode currentDisplayMode;
    SDL_GetCurrentDisplayMode(0, &currentDisplayMode);

    int windowXPosition = (currentDisplayMode.w / 2) - (width_ / 2);
    int windowYPosition = (currentDisplayMode.h / 2) - (height_ / 2);

    int flags = SDL_WINDOW_SHOWN;

    if (fullscreen_)
        flags |= SDL_WINDOW_FULLSCREEN;

    sdlWindow_ = SDL_CreateWindow(title_.c_str(), windowXPosition, windowYPosition, width_, height_, (Uint32)flags);

    if (sdlWindow_ == nullptr)
    {
        std::cout << "Error creating window: " << SDL_GetError() << std::endl;
        return false;
    }

    rendererAdapter_ = std::make_unique<RendererAdapter>();

    if (!rendererAdapter_->init(sdlWindow_, resolutionWidth, resolutionHeight))
    {
        SDL_DestroyWindow(sdlWindow_);
        return false;
    }

    return true;
}

unsigned int milk::adapter::WindowAdapter::width() const
{
    return width_;
}

unsigned int milk::adapter::WindowAdapter::height() const
{
    return height_;
}

bool milk::adapter::WindowAdapter::fullscreen() const
{
    return fullscreen_;
}

void milk::adapter::WindowAdapter::toggleFullscreen()
{
    fullscreen_ = !fullscreen_;

    if (!fullscreen_)
    {
        const int WINDOWED = 0;
        SDL_SetWindowFullscreen(sdlWindow_, WINDOWED);
        SDL_SetWindowSize(sdlWindow_, width_, height_);
    }
    else
    {
        int displayIndex = SDL_GetWindowDisplayIndex(sdlWindow_);

        SDL_Rect displayBounds;
        SDL_GetDisplayBounds(displayIndex, &displayBounds);

        nativeWidth_ = (unsigned int)(displayBounds.w);
        nativeHeight_ = (unsigned int)displayBounds.h;

        SDL_SetWindowSize(sdlWindow_, nativeWidth_, nativeHeight_);
        SDL_SetWindowFullscreen(sdlWindow_, SDL_WINDOW_FULLSCREEN);
    }
}

milk::Renderer& milk::adapter::WindowAdapter::renderer() const
{
    return *rendererAdapter_;
}

milk::adapter::RendererAdapter& milk::adapter::WindowAdapter::rendererAdapter() const
{
    return *rendererAdapter_;
}

SDL_Window* milk::adapter::WindowAdapter::sdlWindow() const
{
    SDL_assert(sdlWindow_ != nullptr);
    return sdlWindow_;
}

void milk::adapter::WindowAdapter::free()
{
    SDL_DestroyWindow(sdlWindow_);
    rendererAdapter_->free();
}

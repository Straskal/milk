#include <iostream>

#include "SDL_hints.h"
#include "SDL_render.h"

#include "RendererAdapter.h"

#include "graphics/Texture.h"
#include "math/Rectangle.h"

milk::adapter::RendererAdapter::RendererAdapter()
        : resolutionWidth_(0),
          resolutionHeight_(0)
{
}

bool milk::adapter::RendererAdapter::init(SDL_Window* sdlWindow,
                                          unsigned int resolutionWidth,
                                          unsigned int resolutionHeight)
{
    resolutionWidth_ = resolutionWidth;
    resolutionHeight_ = resolutionHeight;

    sdlRenderer_ = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (sdlRenderer_ == nullptr)
    {
        std::cout << "Error creating renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_RenderSetLogicalSize(sdlRenderer_, resolutionWidth, resolutionHeight);
    SDL_SetRenderDrawColor(sdlRenderer_, 0x00, 0x00, 0x00, 0xFF);
    SDL_SetRenderDrawBlendMode(sdlRenderer_, SDL_BLENDMODE_BLEND);

    return true;
}

void milk::adapter::RendererAdapter::clear()
{
    SDL_SetRenderDrawColor(sdlRenderer_, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(sdlRenderer_);
}

void milk::adapter::RendererAdapter::drawRectangle(milk::Rectangle& destinationRectangle, milk::Color color)
{
    SDL_Rect dst = {destinationRectangle.x, destinationRectangle.y, destinationRectangle.width, destinationRectangle.height};

    SDL_SetRenderDrawColor(sdlRenderer_, color.red, color.blue, color.green, color.alpha);
    SDL_RenderFillRect(sdlRenderer_, &dst);
}

void milk::adapter::RendererAdapter::drawRectangleOutline(milk::Rectangle& destinationRectangle, milk::Color color)
{
    SDL_Rect dst = {destinationRectangle.x, destinationRectangle.y, destinationRectangle.width, destinationRectangle.height};

    SDL_SetRenderDrawColor(sdlRenderer_, color.red, color.blue, color.green, color.alpha);
    SDL_RenderDrawRect(sdlRenderer_, &dst);
}

void milk::adapter::RendererAdapter::draw(milk::Texture& texture,
                                          milk::Rectangle& sourceRectangle,
                                          milk::Rectangle& destinationRectangle,
                                          int flipFlags)
{
    SDL_Rect src = {sourceRectangle.x, sourceRectangle.y, sourceRectangle.width, sourceRectangle.height};
    SDL_Rect dst = {destinationRectangle.x, destinationRectangle.y, destinationRectangle.width, destinationRectangle.height};

    SDL_RenderCopyEx(sdlRenderer_, texture.get(), &src, &dst, 0, nullptr, (SDL_RendererFlip)flipFlags);
}

void milk::adapter::RendererAdapter::present()
{
    SDL_RenderPresent(sdlRenderer_);
}

milk::Resolution milk::adapter::RendererAdapter::resolution() const
{
    Resolution res = {};
    res.width = resolutionWidth_;
    res.height = resolutionHeight_;

    return res;
}

void milk::adapter::RendererAdapter::free()
{
    SDL_DestroyRenderer(sdlRenderer_);
}

SDL_Renderer* milk::adapter::RendererAdapter::sdlRenderer() const
{
    return sdlRenderer_;
}

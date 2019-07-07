#include "graphics.h"

#include <iostream>
#include <string>
#include <unordered_map>

#include "SDL.h"
#include "SDL_image.h"

#include "image.h"
#include "data/uid.h"

/* Renderer info */
static const int FIRST_SUPPORTED_RENDERING_DRIVER = -1;
static const char* BATCHING_ON = "1";
static const char* NEAREST_PIXEL_SAMPLING = "nearest";

static SDL_Renderer* renderer_handle = nullptr;

/* Image cache info */
static const milk::u32 IMG_UID_GEN_BITS = 16;
static const milk::u32 IMG_ID_IDX_BITS = 16;
static const milk::u32 IMG_ID_MAX_FREE_IDX = 1024;
static milk::UID image_uids;
static std::unordered_map<std::string, milk::u32> images_by_path;
static std::unordered_map<milk::u32, milk::ImageData*> images_by_id;

bool milk::graphics_init(void* windowHandle)
{
	renderer_handle = SDL_CreateRenderer((SDL_Window*)windowHandle, FIRST_SUPPORTED_RENDERING_DRIVER, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer_handle == nullptr) {
		std::cout << "Error creating SDL_Renderer: " << SDL_GetError() << std::endl;
		return false;
	}

	/* Default the resolution to the window dimensions. */
	int w, h;
	SDL_GetWindowSize((SDL_Window*)windowHandle, &w, &h);
	SDL_RenderSetLogicalSize(renderer_handle, w, h);

	SDL_SetHint(SDL_HINT_RENDER_BATCHING, BATCHING_ON);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, NEAREST_PIXEL_SAMPLING);
	SDL_SetRenderDrawBlendMode(renderer_handle, SDL_BLENDMODE_BLEND);

	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if ((IMG_Init(flags) & flags) != flags) {
		std::cout << "Error initializing SDL_image: " << IMG_GetError() << std::endl;
		return false;
	}

	return true;
}

void milk::graphics_quit()
{
	for (auto itr : images_by_id) {
		u32 id = images_by_path.at(itr.second->path);
		id::recycle(&image_uids, 16, id);
		SDL_DestroyTexture((SDL_Texture*)itr.second->handle);
		delete itr.second;
	}
	images_by_path.clear();
	images_by_id.clear();
	IMG_Quit();
	SDL_DestroyRenderer(renderer_handle);
}

std::tuple<int, int> milk::graphics_get_resolution()
{
	int w, h;
	SDL_RenderGetLogicalSize(renderer_handle, &w, &h);
	return std::make_tuple(w, h);
}

void milk::graphics_set_resolution(int w, int h)
{
	SDL_RenderSetLogicalSize(renderer_handle, w, h);
}

milk::u32 milk::graphics_load_imagedata(const char* path)
{
	std::unordered_map<std::string, u32>::iterator loaded = images_by_path.find(path);
	if (loaded != images_by_path.end()) {
		++images_by_id.at(loaded->second)->refCount;
		return loaded->second;
	}

	SDL_Surface* sdlsurface = IMG_Load(path);
	if (sdlsurface == nullptr) {
		std::cout << "Error loading image: " << IMG_GetError() << std::endl;
		return 0;
	}

	SDL_Texture* sdltexture = SDL_CreateTextureFromSurface(renderer_handle, sdlsurface);
	SDL_FreeSurface(sdlsurface);

	int w, h;
	SDL_QueryTexture(sdltexture, nullptr, nullptr, &w, &h);

	ImageData* imageData = new ImageData();
	imageData->path = path;
	imageData->refCount = 1;
	imageData->handle = sdltexture;
	imageData->width = w;
	imageData->height = h;

	u32 id = id::make(&image_uids, IMG_UID_GEN_BITS, IMG_ID_MAX_FREE_IDX);
	images_by_path.insert(std::make_pair(path, id));
	images_by_id.insert(std::make_pair(id, imageData));
	return id;
}

milk::ImageData* milk::graphics_get_imagedata(u32 id)
{
	return id::valid(&image_uids, IMG_UID_GEN_BITS, IMG_ID_IDX_BITS, id) ? images_by_id.at(id) : nullptr;
}

void milk::graphics_dereference_imagedata(u32 id)
{
	if (id::valid(&image_uids, 16, 16, id)) {
		ImageData* imageData = images_by_id.at(id);
		if (--imageData->refCount <= 0) {
			images_by_path.erase(imageData->path);
			images_by_id.erase(id);
			SDL_DestroyTexture((SDL_Texture*)imageData->handle);
			delete imageData;

			id::recycle(&image_uids, IMG_UID_GEN_BITS, id);
		}
	}
}

void milk::graphics_clear()
{
	SDL_RenderClear(renderer_handle);
}

void milk::graphics_set_draw_color(double r, double g, double b, double a)
{
	SDL_SetRenderDrawColor(
		renderer_handle, 
		(Uint8)(std::min((int)(r * 0xFF), 0xFF)), 
		(Uint8)(std::min((int)(g * 0xFF), 0xFF)), 
		(Uint8)(std::min((int)(b * 0xFF), 0xFF)), 
		(Uint8)(std::min((int)(a * 0xFF), 0xFF))
	);
}

void milk::graphics_draw_rectangle(float x, float y, float w, float h)
{
	SDL_FRect dest;
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
	SDL_RenderDrawRectF(renderer_handle, &dest);
}

void milk::graphics_draw_rectangle_filled(float x, float y, float w, float h)
{
	SDL_FRect dest;
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
	SDL_RenderFillRectF(renderer_handle, &dest);
}

void milk::graphics_draw(const Image* image, float x, float y)
{
	ImageData* imageData = images_by_id.at(image->uid);
	SDL_Texture* texture = (SDL_Texture*)imageData->handle;

	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer_handle, &r, &g, &b, &a);
	SDL_SetTextureColorMod(texture, r, g, b);
	SDL_SetTextureAlphaMod(texture, a);

	int w = imageData->width;
	int h = imageData->height;

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = w;
	src.h = h;

	SDL_FRect dest;
	dest.x = x;
	dest.y = y;
	dest.w = (float)w;
	dest.h = (float)h;

	SDL_RenderCopyExF(renderer_handle, texture, &src, &dest, 0, nullptr, SDL_FLIP_NONE);
}

void milk::graphics_draw(const Image* image, float x, float y, int srcx, int srcy, int srcw, int srch, float scx, float scy, double angle)
{
	ImageData* imageData = images_by_id.at(image->uid);
	SDL_Texture* texture = (SDL_Texture*)imageData->handle;

	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer_handle, &r, &g, &b, &a);
	SDL_SetTextureColorMod(texture, r, g, b);
	SDL_SetTextureAlphaMod(texture, a);

	SDL_Rect src;
	src.x = srcx;
	src.y = srcy;
	src.w = srcw;
	src.h = srch;

	Uint8 flip = SDL_FLIP_NONE;
	if (scx < 0) {
		flip |= SDL_FLIP_HORIZONTAL;
		scx *= -1;
	}
	if (scy < 0) {
		flip |= SDL_FLIP_VERTICAL;
		scy *= -1;
	}

	SDL_FRect dest;
	dest.x = x;
	dest.y = y;
	dest.w = (float)srcw * scx;
	dest.h = (float)srch * scy;

	SDL_FPoint center;
	center.x = dest.w * 0.5f;
	center.y = dest.h * 0.5f;

	SDL_RenderCopyExF(renderer_handle, texture, &src, &dest, angle, &center, (SDL_RendererFlip)flip);
}

void milk::graphics_present()
{
	SDL_RenderPresent(renderer_handle);
}

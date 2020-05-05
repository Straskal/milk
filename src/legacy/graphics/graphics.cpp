#include "graphics.h"

#include <iostream>
#include <string>
#include <unordered_map>

#include "SDL.h"
#include "SDL_image.h"

#include "image.h"
#include "uid.h"

/* Renderer info */
#define FIRST_SUPPORTED_RENDERING_DRIVER -1
#define BATCHING_ON "1"
#define NEAREST_PIXEL_SAMPLING "nearest"

static SDL_Renderer* renderer_handle = nullptr;

/* image cache info */
static milk::uid_data image_uids;
static std::unordered_map<std::string, milk::u32> images_by_path;
static std::unordered_map<milk::u32, milk::image_data*> images_by_uid;

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
	for (std::pair<u32, image_data*> pair : images_by_uid) {
		uid_free(&image_uids, pair.first);
		SDL_DestroyTexture((SDL_Texture*)pair.second->handle);
		delete pair.second;
	}
	images_by_uid.clear();
	images_by_path.clear();

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
		++images_by_uid.at(loaded->second)->ref_count;
		return loaded->second;
	}

	SDL_Surface* sdl_surface = IMG_Load(path);
	if (sdl_surface == nullptr) {
		std::cout << "Error loading image: " << IMG_GetError() << std::endl;
		return milk::INVALID_UID;
	}

	SDL_Texture* sdl_texture = SDL_CreateTextureFromSurface(renderer_handle, sdl_surface);
	SDL_FreeSurface(sdl_surface);

	int w, h;
	SDL_QueryTexture(sdl_texture, nullptr, nullptr, &w, &h);

	image_data* imgdata = new image_data();
	imgdata->path = path;
	imgdata->ref_count = 1;
	imgdata->handle = sdl_texture;
	imgdata->width = w;
	imgdata->height = h;

	u32 uid = uid_new(&image_uids);
	images_by_path.insert(std::make_pair(path, uid));
	images_by_uid.insert(std::make_pair(uid, imgdata));
	return uid;
}

milk::image_data* milk::graphics_get_imagedata(u32 uid)
{
	return uid_alive(&image_uids, uid) ? images_by_uid.at(uid) : nullptr;
}

void milk::graphics_dereference_imagedata(u32 uid)
{
	if (uid_alive(&image_uids, uid)) {
		image_data* imgdata = images_by_uid.at(uid);
		if (--imgdata->ref_count <= 0) {
			images_by_path.erase(imgdata->path);
			images_by_uid.erase(uid);
			SDL_DestroyTexture((SDL_Texture*)imgdata->handle);
			delete imgdata;

			uid_free(&image_uids, uid);
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
	SDL_FRect dest = { x, y, w, h };
	SDL_RenderDrawRectF(renderer_handle, &dest);
}

void milk::graphics_draw_rectangle_filled(float x, float y, float w, float h)
{
	SDL_FRect dest = { x, y, w, h };
	SDL_RenderFillRectF(renderer_handle, &dest);
}

void milk::graphics_draw(const image* image, float x, float y)
{
	image_data* imgdata = images_by_uid.at(image->uid);
	SDL_Texture* texture = (SDL_Texture*)imgdata->handle;

	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer_handle, &r, &g, &b, &a);
	SDL_SetTextureColorMod(texture, r, g, b);
	SDL_SetTextureAlphaMod(texture, a);

	int w = imgdata->width;
	int h = imgdata->height;

	SDL_Rect src = { 0, 0, w, h };
	SDL_FRect dest = { x, y, (float)w, (float)h };
	SDL_RenderCopyExF(renderer_handle, texture, &src, &dest, 0, nullptr, SDL_FLIP_NONE);
}

void milk::graphics_draw(const image* image, float x, float y, int srcx, int srcy, int srcw, int srch, float scx, float scy, double angle)
{
	image_data* imgdata = images_by_uid.at(image->uid);
	SDL_Texture* texture = (SDL_Texture*)imgdata->handle;

	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer_handle, &r, &g, &b, &a);
	SDL_SetTextureColorMod(texture, r, g, b);
	SDL_SetTextureAlphaMod(texture, a);

	Uint8 flip = SDL_FLIP_NONE;
	if (scx < 0) {
		flip |= SDL_FLIP_HORIZONTAL;
		scx *= -1;
	}
	if (scy < 0) {
		flip |= SDL_FLIP_VERTICAL;
		scy *= -1;
	}

	SDL_Rect src = { srcx, srcy, srcw, srch };
	SDL_FRect dest = { x, y, (float)srcw * scx, (float)srch * scy };
	SDL_FPoint center = { dest.w * 0.5f, dest.h * 0.5f };
	SDL_RenderCopyExF(renderer_handle, texture, &src, &dest, angle, &center, (SDL_RendererFlip)flip);
}

void milk::graphics_present()
{
	SDL_RenderPresent(renderer_handle);
}

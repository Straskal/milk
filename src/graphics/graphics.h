#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <tuple>

#include "int.h"

namespace milk 
{
	struct Image;
	struct ImageData;

	bool graphics_init(void* windowHandle);
	void graphics_quit();

	std::tuple<int, int> graphics_get_resolution();
	void graphics_set_resolution(int w, int h);

	u32 graphics_load_imagedata(const char* path);
	ImageData* graphics_get_imagedata(u32 id);
	void graphics_dereference_imagedata(u32 id);

	void graphics_clear();
	void graphics_set_draw_color(double r, double g, double b, double a);
	void graphics_draw_rectangle(float x, float y, float w, float h);
	void graphics_draw_rectangle_filled(float x, float y, float w, float h);
	void graphics_draw(const Image* image, float x, float y);
	void graphics_draw(const Image* image, float x, float y, int srcx, int srcy, int srcw, int srch, float scx, float scy, double angle);
	void graphics_present();
}

#endif
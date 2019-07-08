#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <tuple>

namespace milk 
{
	bool window_init();
	void window_quit();

	void* window_get_handle();

	void window_poll();

	void window_show();
	const char* window_get_title();
	void window_set_title(const char* title);
	void window_set_icon(const char* filepath);
	std::tuple<int, int> window_get_size();
	void window_set_size(int width, int height);
	bool window_get_is_fullscreen();
	void window_set_is_fullscreen(bool toggle);
	void window_minimize();
	void window_restore();
	void window_close();
	bool window_should_close();
}

#endif
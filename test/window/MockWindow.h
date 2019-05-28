#ifndef MILK_MOCK_WINDOW_H
#define MILK_MOCK_WINDOW_H

#include "gmock/gmock.h"
#include "window/Window.h"

namespace milk {
	class MockWindow : public Window {
	public:
		MOCK_METHOD4(init, bool(const std::string&, int, int, bool));
		MOCK_CONST_METHOD0(fullscreen, bool());
		MOCK_METHOD0(toggleFullscreen, void());
		MOCK_CONST_METHOD0(handle, void* ());
		MOCK_METHOD0(free, void());
	};
}

#endif
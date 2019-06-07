#ifndef MILK_LOCATOR_H
#define MILK_LOCATOR_H

#include "window/Window.h"
#include "window/Renderer.h"

namespace milk {
	struct Locator {
		static Window* window;
		static Renderer* renderer;
	};
}

#endif
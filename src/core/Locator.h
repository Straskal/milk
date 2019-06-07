#ifndef MILK_LOCATOR_H
#define MILK_LOCATOR_H

#include "input/Keyboard.h"
#include "window/Window.h"
#include "window/Renderer.h"

/*
	The Locators intended usage is for lua modules only.
*/
namespace milk {
	struct Locator {
		static Window* window;
		static Renderer* renderer;
		static Keyboard* keyboard;
	};
}

#endif
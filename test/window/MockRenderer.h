#ifndef MILK_MOCK_RENDERER_H
#define MILK_MOCK_RENDERER_H

#include "gmock/gmock.h"
#include "graphics/Color.h"
#include "graphics/Texture.h"
#include "math/Rectangle.h"
#include "window/Renderer.h"

namespace milk {
	class MockRenderer : public Renderer {
	public:
		MOCK_METHOD3(init, bool(void*, int, int));
		MOCK_METHOD1(clear, void(const Color&));
		MOCK_METHOD2(drawRectangle, void(const Rectangle&, const Color&));
		MOCK_METHOD4(draw, void(const Texture&, const Rectangle&, const Rectangle&, U8));
		MOCK_METHOD0(present, void());
		MOCK_CONST_METHOD0(resolution, Resolution());
		MOCK_METHOD0(free, void());
	};
}

#endif
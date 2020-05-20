/*
 *  MIT License
 *
 *  Copyright(c) 2018 - 2020 Stephen Traskal
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software andassociated documentation files(the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, andto permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions :
 *
 *  The above copyright notice andthis permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include <math.h>
#include <memory.h>
#include <stdio.h>

#include "milk.h"
#include "milk_api.h"

Milk *milkInit()
{
	Milk *milk = (Milk *)calloc(1, sizeof(Milk));
	milkOpenAudio(&milk->audio);
	milkOpenVideo(&milk->video);
	milkLoadScripts(milk);
	return milk;
}

void milkFree(Milk *milk)
{
	milkUnloadScripts(milk);
	milkCloseAudio(&milk->audio);
	free(milk);
}

void milkUpdate(Milk *milk)
{
	milkInvokeUpdate(&milk->code);
}

static void _resetDrawState(Video *video)
{
	video->colorKey = 0;
	video->clipRect.top = 0;
    video->clipRect.left = 0;
	video->clipRect.bottom = MILK_FRAMEBUF_HEIGHT;
	video->clipRect.right = MILK_FRAMEBUF_WIDTH;
}

void milkDraw(Milk *milk)
{
	_resetDrawState(&milk->video);
	milkInvokeDraw(&milk->code);
}

int milkButton(Input *input, uint8_t button)
{
	return (input->gamepad.buttonState & button) == button;
}

int milkButtonPressed(Input *input, uint8_t button)
{
	return (input->gamepad.buttonState & button) == button && (input->gamepad.previousButtonState & button) != button;
}

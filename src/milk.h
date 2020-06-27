/*
 *  MIT License
 *
 *  Copyright(c) 2018 - 2020 Stephen Traskal
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files(the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions :
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#ifndef __MILK_H__
#define __MILK_H__

#include <stdbool.h>

#include "audio.h"
#include "common.h"
#include "gamepad.h"
#include "video.h"

typedef struct code
{
    void *state;
} Code;

typedef struct milk
{
	Input   input;
    Video   video;
    Audio   audio;
	Code    code;
    bool    shouldQuit;
} Milk;

/**
 * Create a new instance of milk and initialize all submodules.
 * @return
 */
Milk *createMilk();

/**
 * Free milk and all of it's submodules.
 * @param milk
 */
void freeMilk(Milk *milk);

/**
 * Load scripts into memory.
 * @param milk
 */
void loadCode(struct milk *milk);

/**
 * Unload all scripts from memory.
 * @param milk
 */
void unloadCode(struct milk *milk);

/**
 * Invoke the _init callback in scripts.
 * @param code
 */
void invokeInit(Code *code);

/**
 * Invoke the _update callback in scripts.
 * @param code
 */
void invokeUpdate(Code *code);

/**
 * Invoke the _draw callback in scripts.
 * @param code
 */
void invokeDraw(Code *code);

#endif

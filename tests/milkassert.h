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

#ifndef __MILK_ASSERT_H__
#define __MILK_ASSERT_H__

#include "milk.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/*
 *******************************************************************************
 * Helpers. Some of which are just for synactic sugar.
 *******************************************************************************
 */
#define TEST_CASE(test)					static void test()
#define SETUP(milk)						Milk *milk = createMilk()
#define ACT(action)                     action
#define FREE_MILK(milk)					freeMilk(milk);
#define TEARDOWN    					teardown

/*
 *******************************************************************************
 * Test struct
 *******************************************************************************
 */

typedef void(*TestFunction)(); /* The signature for all test cases. */

typedef struct test
{
	char *name;
    char *failedAssert;
	TestFunction execute;
} Test;

Test *gCurrentTest = NULL;

#define INIT_TEST(func) { #func, NULL, func }

/*
 *******************************************************************************
 * Asserts
 *******************************************************************************
 */

#define BASE_ASSERT(condition)\
	do {\
		if (!(condition)) {\
			gCurrentTest->failedAssert = #condition;\
			goto teardown;\
		}\
	} while(0)

#define ASSERT_TRUE(val)				BASE_ASSERT(val)
#define ASSERT_FALSE(val)				BASE_ASSERT(!(val))
#define ASSERT_EQ(expected, actual)		BASE_ASSERT((expected) == (actual))
#define ASSERT_NEQ(expected, actual)	BASE_ASSERT((expected) != (actual))
#define ASSERT_NULL(val)				BASE_ASSERT((val) == (NULL))
#define ASSERT_NNULL(val)				BASE_ASSERT((val) != (NULL))

/*
 *******************************************************************************
 * Runner
 *******************************************************************************
 */

int runTests(Test *tests, size_t count)
{
	size_t passedCount = 0;

	printf("Running tests\n\n\n");

	for (size_t i = 0; i < count; i++)
	{
		gCurrentTest = &tests[i];
		tests[i].execute();
	}

	for (size_t i = 0; i < count; i++)
		if (tests[i].failedAssert == NULL)
			passedCount++;

	printf("Passed %d/%d\n", passedCount, count);
	printf("=======================================\n\n");

	for (size_t i = 0; i < count; i++)
		if (tests[i].failedAssert == NULL)
			printf("	- %s\n", tests[i].name);

	printf("\nFailed %d/%d\n", count - passedCount, count);
	printf("=======================================\n\n");

    for (size_t i = 0; i < count; i++)
        if (tests[i].failedAssert != NULL)
        {
            printf("	- %s\n", tests[i].name);
            printf("		%s\n", tests[i].failedAssert);
        }

	return !(passedCount == count);
}

#endif

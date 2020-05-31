#ifndef __MILK_ASSERT_H__
#define __MILK_ASSERT_H__

#include "milk.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/*
 *******************************************************************************
 * Helpers
 *******************************************************************************
 */
#define TEST_METHOD(test)				static void test()
#define ARRANGE_MILK(milk)				Milk *milk = milkCreate()
#define ACT(action) action
#define FREE_MILK(milk)					milkFree(milk)

/*
 *******************************************************************************
 * Test
 *******************************************************************************
 */

typedef void(*TestFunction)(); /* The signature for all test cases. */

typedef struct test
{
	char *name;
	TestFunction testFunc;
	char *failedAsserts[8];
	int failedAssertCount;
} Test;

Test *gCurrentTest = NULL; /* The current test. */

/*
 *******************************************************************************
 * Asserts
 *******************************************************************************
 */

#define BASE_ASSERT(condition)\
	do {\
		if (!(condition))\
			gCurrentTest->failedAsserts[gCurrentTest->failedAssertCount++] = #condition;\
	} while(0)

#define ASSERT_TRUE(condition)			BASE_ASSERT(condition)
#define ASSERT_FALSE(condition)			BASE_ASSERT(!(condition))
#define ASSERT_EQ(expected, actual)		BASE_ASSERT(expected == actual)
#define ASSERT_NEQ(expected, actual)	BASE_ASSERT(expected != actual)
#define ASSERT_NNULL(val)				BASE_ASSERT(val != NULL)

/*
 *******************************************************************************
 * Runner
 *******************************************************************************
 */

int runTests(Test *tests, size_t count)
{
	int passedCount = 0;

	printf("Running tests\n\n\n");

	for (size_t i = 0; i < count; i++)
	{
		tests[i].failedAssertCount = 0;
		gCurrentTest = &tests[i];
		tests[i].testFunc();
	}

	for (size_t i = 0; i < count; i++)
		if (tests[i].failedAssertCount == 0)
			passedCount++;

	printf("Passed %d/%d\n", passedCount, count);
	printf("=======================================\n\n");

	for (size_t i = 0; i < count; i++)
		if (tests[i].failedAssertCount == 0)
			printf("	- %s\n", tests[i].name);

	printf("\nFailed %d/%d\n", count - passedCount, count);
	printf("=======================================\n\n");

	for (size_t i = 0; i < count; i++)
		if (tests[i].failedAssertCount > 0)
		{
			printf("	- %s\n", tests[i].name);

			for (int j = 0; j < tests[i].failedAssertCount; j++)
				printf("		%s\n", tests[i].failedAsserts[j]);
		}

	return !(passedCount == count);
}

#define RUN_ALL_TESTS(tests, count) runTests(tests, count)

#endif
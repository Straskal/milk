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
#define ACT(action)                     action

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
#define END_ASSERTS()                   teardown: ((void)0)

 /*
	*******************************************************************************
	* Runner
	*******************************************************************************
	*/

static void executeTests(Test *tests, int count)
{
	printf("Running tests\n\n\n");

	for (int i = 0; i < count; i++)
	{
		gCurrentTest = &tests[i];
		tests[i].execute();
	}
}

static int printPassRate(Test *tests, int count)
{
	int passRate = 0;
	for (int i = 0; i < count; i++)
		if (tests[i].failedAssert == NULL)
			passRate++;

	printf("Passed %d/%d\n", passRate, count);
	printf("=======================================\n\n");

	for (int i = 0; i < count; i++)
		if (tests[i].failedAssert == NULL)
			printf("	- %s\n", tests[i].name);

	return passRate;
}

static void printFailedRate(Test *tests, int count, int passRate)
{
	printf("\nFailed %d/%d\n", count - passRate, count);
	printf("=======================================\n\n");

	for (int i = 0; i < count; i++)
	{
		if (tests[i].failedAssert != NULL)
		{
			printf("	- %s\n", tests[i].name);
			printf("		%s\n", tests[i].failedAssert);
		}
	}
}

int runTests(Test *tests, int count)
{
	executeTests(tests, count);
	int passRate = printPassRate(tests, count);
	printFailedRate(tests, count, passRate);
	return passRate < count ? -1 : 0;
}

#endif

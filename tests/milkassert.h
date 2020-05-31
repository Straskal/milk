#ifndef __MILK_ASSERT_H__
#define __MILK_ASSERT_H__

#include "milk.h"

/* Test helpers */
#define TEST_METHOD(test)				static void test()
#define ARRANGE_MILK(milk)				Milk *milk = milkCreate()
#define ACT(action) action
#define FREE_MILK(milk)					milkFree(milk)

typedef void(*TestFunction)(); /* The signature for all test cases. */

typedef struct test
{
	char *name;
	TestFunction testFunc;
	char *failedAsserts[8];
	int failedAssertCount;
} Test;

static Test *gCurrentTest = NULL; /* The current test. */

/* Asserts */
#define BASE_ASSERT(condition)\
	do {\
		if (!(condition))\
			gCurrentTest->failedAsserts[gCurrentTest->failedAssertCount++] = #condition;\
	} while(0)

#define ASSERT_TRUE(condition)			BASE_ASSERT(condition)
#define ASSERT_FALSE(condition)			BASE_ASSERT(!(condition))
#define ASSERT_EQ(expected, actual)		BASE_ASSERT(expected == actual)
#define ASSERT_NEQ(expected, actual)	BASE_ASSERT(expected != actual)
#define ASSERT_NNULL(val)			BASE_ASSERT(val != NULL)

#endif

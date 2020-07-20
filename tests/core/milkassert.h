#ifndef __MILK_ASSERT_H__
#define __MILK_ASSERT_H__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "milk.h"

// Syntactic sugar for defining test cases.
#define TEST_CASE(test) void test()

// Syntactic sugar for defining the act portion of test.
#define ACT(action) action

typedef struct
{
	char *name;

	// The test has failed if the assert is not null.
	char *failedAssert;

  void(*execute)();
} Test;

Test *gCurrentTest = NULL;

#define INIT_TEST(func) { #func, NULL, func }

// Base assert just sets the failed assert message to the literal given condition.
// This is pretty rudimentary. Should probably consider also printing the type of assert that failed.
#define BASE_ASSERT(condition)\
	do {\
		if (!(condition)) {\
			gCurrentTest->failedAssert = #condition;\
			goto teardown;\
		}\
	} while(0)

#define ASSERT_TRUE(val)				      BASE_ASSERT(val)
#define ASSERT_FALSE(val)				      BASE_ASSERT(!(val))
#define ASSERT_EQ(expected, actual)	  BASE_ASSERT((expected) == (actual))
#define ASSERT_NEQ(expected, actual)  BASE_ASSERT((expected) != (actual))
#define ASSERT_NULL(val)				      BASE_ASSERT((val) == (NULL))

// The assert system falls back to the 'teardown' label upon the first failed assertion.
// This macro just makes the tests look a bit nicer.
#define END_ASSERTS() teardown: ((void)0)

static int executeTests(Test *tests, int count) {
  int passRate = 0;
	printf("Running tests\n\n\n");
	for (int i = 0; i < count; i++) {
		gCurrentTest = &tests[i];
		tests[i].execute();
		if (tests[i].failedAssert == NULL)
		  passRate++;
	}
  return passRate;
}

static int printPassRate(Test *tests, int count, int passRate) {
	printf("Passed %d/%d\n", passRate, count);
	printf("=======================================\n\n");
	for (int i = 0; i < count; i++) {
    if (tests[i].failedAssert == NULL)
      printf("	- %s\n", tests[i].name);
  }
	return passRate;
}

static void printFailedRate(Test *tests, int count, int passRate) {
	printf("\nFailed %d/%d\n", count - passRate, count);
	printf("=======================================\n\n");
	for (int i = 0; i < count; i++)	{
		if (tests[i].failedAssert != NULL) {
			printf("	- %s\n", tests[i].name);
			printf("		%s\n", tests[i].failedAssert);
		}
	}
}

int runTests(Test *tests, int count) {
  int passRate = executeTests(tests, count);
	printPassRate(tests, count, passRate);
	printFailedRate(tests, count, passRate);
	return passRate < count ? -1 : 0;
}

#endif

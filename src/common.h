#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>

// Defining your own types is trendy.
typedef int16_t   s16;
typedef int32_t   s32;
typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;

// Helpful macro to avoid analysis warnings.
#define UNUSED(v) ((void)v)

// Evaluates to the lower value of the two.
#define MIN(x, y) ((x) > (y) ? (y) : (x))

// Evaluates to the higher value of the two.
#define MAX(x, y) ((x) > (y) ? (x) : (y))

// Clamps the given value between the lower and higher boundaries.
#define CLAMP(v, low, up) (MAX(low, MIN(v, up)))

// Evaluates to -1 for negative numbers, and 0 or 1 for positive numbers.
#define SIGN(x) ((x > 0) - (x < 0))

// Evaluates to true if the mask contains the given bit.
#define IS_BIT_SET(val, bit) ((val & bit) > 0)

#endif

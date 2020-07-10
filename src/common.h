#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>

typedef int16_t   s16;
typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;

#define UNUSED(v) ((void)v)
#define MIN(x, y) ((x) > (y) ? (y) : (x))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define CLAMP(v, low, up) (MAX(low, MIN(v, up)))
#define SIGN(x) ((x > 0) - (x < 0))
#define IS_BIT_SET(val, bit) ((val & bit) > 0)

#endif

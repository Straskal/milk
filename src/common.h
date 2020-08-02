#ifndef __COMMON_H__
#define __COMMON_H__

#include <math.h>

#define UNUSED(v) ((void)v)

#define MIN(x, y) ((x) > (y) ? (y) : (x))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define CLAMP(v, low, up) (MAX(low, MIN(v, up)))
#define FLOOR(x) (floor((double)(x)))
#define SIGN(x) ((x > 0) - (x < 0))

#define SET_BIT(val, bit) (val |= bit)
#define UNSET_BIT(val, bit) (val &= ~bit)
#define TOGGLE_BIT(val, bit) (val ^= bit)
#define CHECK_BIT(val, bit) ((val & bit) > 0)

#endif

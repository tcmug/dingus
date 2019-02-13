#ifndef TW_MATH_H
#define TW_MATH_H

#include <math.h>

#define PI (3.14159265359)
#define PI2 (PI * 2)
#define PI_240 (PI * (240.0 / 180.0))
#define PI_120 (PI * (120.0 / 180.0))
#define HALF_PI (PI / 2)

#define TO_RAD(degrees) ((degrees)*0.01745329251994)
#define TO_DEG(radians) ((radians)*57.29577951308233)

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define EPSILON (0.0001f)
#define MAX_DISTANCE (1000000)

#include "types.h"

#include "functions.h"

#endif
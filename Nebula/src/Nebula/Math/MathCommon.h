#pragma once

#include <assert.h>
#include <math.h>

namespace Nebula
{
    #define MATH_PI 3.141592654f
    #define PI2R_FACTOR MATH_PI / 180.0f
    #define DEG_TO_RAD(x) (x * MATH_PI) / 180.0f
    #define RAD_TO_DEG(x) (x * 180.0f) / MATH_PI
}
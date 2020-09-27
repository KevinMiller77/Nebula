#pragma once
#include <cstring>
#define PI 3.14159265359f
#define PI2R_FACTOR PI / 180

//A compare for map in order to use a c string
struct StrCompare {
public:
    bool operator() (const char* str1, const char* str2) const { return std::strcmp(str1, str2) < 0; }
};

#include "vec2f.h"
#include "vec2u.h"
#include "vec3f.h"
#include "vec4f.h"
#include "mat2f.h"
#include "mat3f.h"
#include "mat4f.h"

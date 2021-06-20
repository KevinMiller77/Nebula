#pragma once

#include "vec3.h"

namespace Nebula {
    struct AABB {
        Vec3f Min = {0.0f, 0.0f, 0.0f};
        Vec3f Max = {0.0f, 0.0f, 0.0f};

        AABB() = default;
        AABB(Vec3f& min, Vec3f& max) : Min(min), Max(max) {}
    };
}

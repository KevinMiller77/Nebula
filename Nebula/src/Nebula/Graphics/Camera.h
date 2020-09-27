#pragma once

#include <Math/math.h>

namespace Nebula
{
    class Camera
    {
    public:
        Camera() = default;
        Camera(const Mat4f projection)
            : Projection(projection) {}

        virtual ~Camera() = default;

        const Mat4f GetProjection() const { return Projection; }

    private:
        Mat4f Projection;
    };
}
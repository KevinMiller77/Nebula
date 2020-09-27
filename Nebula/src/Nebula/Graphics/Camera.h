#pragma once

#include <Math/math.h>

namespace Nebula
{
    class Camera
    {
    public:
        Camera() = default;
        Camera(const Mat4f projection)
            : ViewProjectionMatrix(projection) {}

        virtual ~Camera() = default;

        const Mat4f GetViewProjection() const { return ViewProjectionMatrix; }

    protected:
        Mat4f ViewProjectionMatrix;
    };
}
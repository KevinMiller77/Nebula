#pragma once

#include <Math/math.h>

namespace Nebula{
    class Camera
    {
    public:
        Camera() = default;
        Camera(const Mat4f projection)
            : m_Projection(projection) {}

        virtual ~Camera() = default;

        Mat4f& GetProjection() const { return (Mat4f&)m_Projection; }
        
        virtual const Mat4f& GetViewMatrix() const = 0;
        virtual const Mat4f GetViewProjection() const = 0;

    protected:
        Mat4f m_Projection = Mat4f(1.0f);
    };
}
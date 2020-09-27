#pragma once

#include "vec3f.h"

//4 * 4 float matrix
struct Mat3f
{
    union {
        float elements[3 * 3];
        Vec3f rows[3];
    };

    Mat3f();
    Mat3f(float identity);
    Mat3f(Vec3f row0, Vec3f row1, Vec3f row3);
    ~Mat3f();

    const Vec3f &operator[](int index) const;
    Vec3f &operator[](int index);

    void operator+=(const Mat3f &other);
    void operator-=(const Mat3f &other);
    void operator*=(const Mat3f &other);

    friend Mat3f operator+(Mat3f left, const Mat3f &right);
    friend Mat3f operator-(Mat3f left, const Mat3f &right);
    friend Mat3f operator*(Mat3f left, const Mat3f &right);

    const float* GetPtr() const { return &elements[0]; }

private:
    void add(const Mat3f &other);
    void subtract(const Mat3f &other);
    void multiply(const Mat3f &other);
};

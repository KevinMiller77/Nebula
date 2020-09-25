#ifndef __KevEngine_MAT4__
#define __KevEngine_MAT4__

#include <math.h>
#include "vec2f.h"
#include "vec3f.h"
#include "vec4f.h"

//4 * 4 float matrix
struct Mat4f
{
    union {
        float elements[4 * 4];
        Vec4f rows[4];
    };

    Mat4f();
    Mat4f(float identity);
    Mat4f(Vec4f row0, Vec4f row1, Vec4f row2, Vec4f row3);
    ~Mat4f();

    static Mat4f orthographic(float left, float right, float top, float bottom, float near, float far);
    static Mat4f perspective(float fov, float aspectRatio, float near, float far);

    static Mat4f translation(const Vec3f &translation);
    static Mat4f rotation(float angle, const Vec3f &axis);
    static Mat4f scale(const Vec3f &scale);

    Mat4f invertMatrix();

    const Vec4f &operator[](int index) const;
    Vec4f &operator[](int index);

    void operator+=(const Mat4f &other);
    void operator-=(const Mat4f &other);
    void operator*=(const Mat4f &other);

    friend Mat4f operator+(Mat4f left, const Mat4f &right);
    friend Mat4f operator-(Mat4f left, const Mat4f &right);
    friend Mat4f operator*(Mat4f left, const Mat4f &right);
    friend Vec4f operator*(Mat4f left, const Vec4f &right);
    friend Vec3f operator*(Mat4f left, const Vec3f &right);
    friend Mat4f operator*(Mat4f left, const double &right);

    void multiply(const Mat4f &other);
    void multiplyLeft(const Mat4f &other);
    Vec4f multiply(const Vec4f &other);
    Vec3f multiply(const Vec3f &other);

    void printMatrix();
    
private:
    void add(const Mat4f &other);
    void subtract(const Mat4f &other);
};

#endif
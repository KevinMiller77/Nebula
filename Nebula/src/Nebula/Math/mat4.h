#ifndef __KevEngine_MAT4__
#define __KevEngine_MAT4__

#include <math.h>
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

//4 * 4 float matrix
struct Mat4 {
    union {
        float elements[4 * 4];
        Vec4 rows[4];
    };
    
    Mat4();
    Mat4(float identity);
    Mat4(Vec4 row0, Vec4 row1, Vec4 row2, Vec4 row3);
    ~Mat4();

    static Mat4 orthographic(float left, float right, float top, float bottom, float near, float far);
	static Mat4 perspective(float fov, float aspectRatio, float near, float far);

	static Mat4 translation(const Vec3& translation);
	static Mat4 rotation(float angle, const Vec3& axis);
	static Mat4 scale(const Vec3& scale);
    
    const Vec4& operator[](int index) const;
    Vec4& operator[](int index);
    
    void operator+=(const Mat4& other);
    void operator-=(const Mat4& other);
    void operator*=(const Mat4& other);
    
    friend Mat4 operator+(Mat4 left, const Mat4& right);
    friend Mat4 operator-(Mat4 left, const Mat4& right);
    friend Mat4 operator*(Mat4 left, const Mat4& right);
    
    private:
    void add(const Mat4& other);
    void subtract(const Mat4& other);
    void multiply(const Mat4& other);
};

#endif
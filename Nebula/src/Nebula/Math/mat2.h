#ifndef __KevEngine_Mat2__
#define __KevEngine_Mat2__

#include "vec2.h"

//4 * 4 float matrix
struct Mat2 {
    union {
        float elements[2 * 2];
        Vec2 rows[2];
    };
    
    Mat2();
    Mat2(float identity);
    Mat2(Vec2 row0, Vec2 row1);
    ~Mat2();
    
    const Vec2& operator[](int index) const;
    Vec2& operator[](int index);
    
    
    void operator+=(const Mat2& other);
    void operator-=(const Mat2& other);
    void operator*=(const Mat2& other);
    
    friend Mat2 operator+(Mat2 left, const Mat2& right);
    friend Mat2 operator-(Mat2 left, const Mat2& right);
    friend Mat2 operator*(Mat2 left, const Mat2& right);
    
    private:
    void add(const Mat2& other);
    void subtract(const Mat2& other);
    void multiply(const Mat2& other);
};

#endif
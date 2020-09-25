#include "mat2f.h"

#include <string.h>
#include <assert.h>

Mat2f::Mat2f() {
    memset(elements, 0, 2 * 2 * sizeof(float));
}

Mat2f::Mat2f(float identity) {
    memset(elements, 0, 2 * 2 * sizeof(float));
    elements[0 * 2 + 0] = identity;
    elements[1 * 2 + 1] = identity;
}

Mat2f::Mat2f(Vec2f row0, Vec2f row1) {
    memcpy(rows[0].elements, row0.elements, 2 * sizeof(float));
    memcpy(rows[1].elements, row1.elements, 2 * sizeof(float));
}

Mat2f::~Mat2f() {}

void Mat2f::add(const Mat2f& other) {
    for (int i = 0; i < 2 * 2; i++) {
        elements[i] += other.elements[i];
    }
}

void Mat2f::subtract(const Mat2f& other) {
    for (int i = 0; i < 2 * 2; i++) {
        elements[i] -= other.elements[i];
    }
}

void Mat2f::multiply(const Mat2f& other) {
    float result[4];
    
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 2; col++) {
            //Dot-product for each element of result
            float resultSum = 0;
            for (int i = 0; i < 2; i++) {
                resultSum += elements[row * 2 + i] * other.elements[i * 2 + col];
            }
            result[row * 2 + col] = resultSum;
        }
    }
    
    memcpy(elements, result, 2 * 2 * sizeof(float));
}

const Vec2f& Mat2f::operator[](int index) const {
    assert(index >= 0 && index < 2);
    
    return rows[index];
}

Vec2f& Mat2f::operator[](int index) {
    assert(index >= 0 && index < 2);
    
    return rows[index];
}

void Mat2f::operator+=(const Mat2f& other) {
    add(other);
}

void Mat2f::operator-=(const Mat2f& other) {
    subtract(other);
}

void Mat2f::operator*=(const Mat2f& other) {
    multiply(other);
}

Mat2f operator+(Mat2f left, const Mat2f& right) {
    left.add(right);
    return left;
}

Mat2f operator-(Mat2f left, const Mat2f& right) {
    left.subtract(right);
    return left;
}

Mat2f operator*(Mat2f left, const Mat2f& right) {
    left.multiply(right);
    return left;
}
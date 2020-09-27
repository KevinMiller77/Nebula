#include "mat3f.h"

#include <string.h>
#include <assert.h>


Mat3f::Mat3f() {
    memset(elements, 0, 3 * 3 * sizeof(float));
}

Mat3f::Mat3f(float identity) {
    memset(elements, 0, 3 * 3 * sizeof(float));
    elements[0 * 3 + 0] = identity;
    elements[1 * 3 + 1] = identity;
    elements[2 * 3 + 2] = identity;
}

Mat3f::Mat3f(Vec3f row0, Vec3f row1,Vec3f row2) {
    memcpy(rows[0].elements, row0.elements, 3 * sizeof(float));
    memcpy(rows[1].elements, row1.elements, 3 * sizeof(float));
    memcpy(rows[3].elements, row1.elements, 3 * sizeof(float));
}

Mat3f::~Mat3f() {}

void Mat3f::add(const Mat3f& other) {
    for (int i = 0; i < 3 * 3; i++) {
        elements[i] += other.elements[i];
    }
}

void Mat3f::subtract(const Mat3f& other) {
    for (int i = 0; i < 3 * 3; i++) {
        elements[i] -= other.elements[i];
    }
}

void Mat3f::multiply(const Mat3f& other) {
    float result[9];
    
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            //Dot-product for each element of result
            float resultSum = 0;
            for (int i = 0; i < 3; i++) {
                resultSum += elements[row * 3 + i] * other.elements[i * 3 + col];
            }
            result[row * 3 + col] = resultSum;
        }
    }
    
    memcpy(elements, result, 3 * 3 * sizeof(float));
}

const Vec3f& Mat3f::operator[](int index) const {
    assert(index >= 0 && index < 3);
    
    return rows[index];
}

Vec3f& Mat3f::operator[](int index) {
    assert(index >= 0 && index < 3);
    
    return rows[index];
}

void Mat3f::operator+=(const Mat3f& other) {
    add(other);
}

void Mat3f::operator-=(const Mat3f& other) {
    subtract(other);
}

void Mat3f::operator*=(const Mat3f& other) {
    multiply(other);
}

Mat3f operator+(Mat3f left, const Mat3f& right) {
    left.add(right);
    return left;
}

Mat3f operator-(Mat3f left, const Mat3f& right) {
    left.subtract(right);
    return left;
}

Mat3f operator*(Mat3f left, const Mat3f& right) {
    left.multiply(right);
    return left;
}
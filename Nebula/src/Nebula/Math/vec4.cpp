#include "vec4.h"

#include <string.h>
#include <assert.h>

Vec4::Vec4() {
    memset(elements, 0, 4 * sizeof(float));
}

Vec4::~Vec4() {}

Vec4::Vec4(float x, float y, float z, float w) {
    elements[0] = x;
    elements[1] = y;
    elements[2] = z;
    elements[3] = w;
}

void Vec4::add(const Vec4& other) {
    elements[0] += other[0];
    elements[1] += other[1];
    elements[2] += other[2];
    elements[3] += other[3];
}

void Vec4::subtract(const Vec4& other) {
    elements[0] -= other[0];
    elements[1] -= other[1];
    elements[2] -= other[2];
    elements[3] -= other[3];
}

void Vec4::multiply(const Vec4& other) {
    elements[0] *= other[0];
    elements[1] *= other[1];
    elements[2] *= other[2];
    elements[3] *= other[3];
}

void Vec4::divide(const Vec4& other) {
    elements[0] /= other[0];
    elements[1] /= other[1];
    elements[2] /= other[2];
    elements[3] /= other[3];
}

const float& Vec4::operator[](int index) const {
    assert(index >= 0 && index < 4);
    
    return elements[index];
}


float& Vec4::operator[](int index) {
    assert(index >= 0 && index < 4);
    
    return elements[index];
}

void Vec4::operator+=(const Vec4& other) {
    add(other);
}

void Vec4::operator-=(const Vec4& other) {
    subtract(other);
}

void Vec4::operator*=(const Vec4& other) {
    multiply(other);
}

void Vec4::operator/=(const Vec4& other) {
    divide(other);
}

Vec4 operator+(Vec4 left, Vec4 right) {
    Vec4 result(left);
    result.add(right);
    return result;
}

Vec4 operator-(Vec4 left, Vec4 right) {
    Vec4 result(left);
    result.subtract(right);
    return result;
}

Vec4 operator*(Vec4 left, Vec4 right) {
    Vec4 result(left);
    result.multiply(right);
    return result;
}

Vec4 operator/(Vec4 left, Vec4 right) {
    Vec4 result(left);
    result.divide(right);
    return result;
}
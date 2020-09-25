#include "vec4f.h"

#include <string.h>
#include <assert.h>

Vec4f::Vec4f() {
    memset(elements, 0, 4 * sizeof(float));
}

Vec4f::~Vec4f() {}

Vec4f::Vec4f(float x, float y, float z, float w) {
    elements[0] = x;
    elements[1] = y;
    elements[2] = z;
    elements[3] = w;
}

void Vec4f::add(const Vec4f& other) {
    elements[0] += other[0];
    elements[1] += other[1];
    elements[2] += other[2];
    elements[3] += other[3];
}

void Vec4f::subtract(const Vec4f& other) {
    elements[0] -= other[0];
    elements[1] -= other[1];
    elements[2] -= other[2];
    elements[3] -= other[3];
}

void Vec4f::multiply(const Vec4f& other) {
    elements[0] *= other[0];
    elements[1] *= other[1];
    elements[2] *= other[2];
    elements[3] *= other[3];
}

void Vec4f::divide(const Vec4f& other) {
    elements[0] /= other[0];
    elements[1] /= other[1];
    elements[2] /= other[2];
    elements[3] /= other[3];
}

const float& Vec4f::operator[](int index) const {
    assert(index >= 0 && index < 4);
    
    return elements[index];
}


float& Vec4f::operator[](int index) {
    assert(index >= 0 && index < 4);
    
    return elements[index];
}

void Vec4f::operator+=(const Vec4f& other) {
    add(other);
}

void Vec4f::operator-=(const Vec4f& other) {
    subtract(other);
}

void Vec4f::operator*=(const Vec4f& other) {
    multiply(other);
}

void Vec4f::operator/=(const Vec4f& other) {
    divide(other);
}

Vec4f operator+(Vec4f left, Vec4f right) {
    Vec4f result(left);
    result.add(right);
    return result;
}

Vec4f operator-(Vec4f left, Vec4f right) {
    Vec4f result(left);
    result.subtract(right);
    return result;
}

Vec4f operator*(Vec4f left, Vec4f right) {
    Vec4f result(left);
    result.multiply(right);
    return result;
}

Vec4f operator/(Vec4f left, Vec4f right) {
    Vec4f result(left);
    result.divide(right);
    return result;
}
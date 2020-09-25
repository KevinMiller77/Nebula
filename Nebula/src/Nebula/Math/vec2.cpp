#include "vec2.h"

#include <string.h>
#include <assert.h>

Vec2::Vec2() {
    memset(elements, 0, 2 * sizeof(float));
}

Vec2::~Vec2() {}

Vec2::Vec2(float x, float y) {
    elements[0] = x;
    elements[1] = y;
}

void Vec2::add(const Vec2& other) {
    elements[0] += other[0];
    elements[1] += other[1];
}

void Vec2::subtract(const Vec2& other) {
    elements[0] -= other[0];
    elements[1] -= other[1];
}

void Vec2::multiply(const Vec2& other) {
    elements[0] *= other[0];
    elements[1] *= other[1];
}

void Vec2::divide(const Vec2& other) {
    elements[0] /= other[0];
    elements[1] /= other[1];
}

const float& Vec2::operator[](int index) const {
    assert(index >= 0 && index < 2);
    
    return elements[index];
}


float& Vec2::operator[](int index) {
    assert(index >= 0 && index < 2);
    
    return elements[index];
}

void Vec2::operator+=(const Vec2& other) {
    add(other);
}

void Vec2::operator-=(const Vec2& other) {
    subtract(other);
}

void Vec2::operator*=(const Vec2& other) {
    multiply(other);
}

void Vec2::operator/=(const Vec2& other) {
    divide(other);
}

Vec2 operator+(Vec2 left, Vec2 right) {
    Vec2 result(left);
    result.add(right);
    return result;
}

Vec2 operator-(Vec2 left, Vec2 right) {
    Vec2 result(left);
    result.subtract(right);
    return result;
}

Vec2 operator*(Vec2 left, Vec2 right) {
    Vec2 result(left);
    result.multiply(right);
    return result;
}

Vec2 operator/(Vec2 left, Vec2 right) {
    Vec2 result(left);
    result.divide(right);
    return result;
}
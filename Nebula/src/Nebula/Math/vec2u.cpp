#include "vec2u.h"

#include <string.h>
#include <assert.h>

Vec2u::Vec2u() {
    memset(elements, 0, 2 * sizeof(unsigned int));
}

Vec2u::~Vec2u() {}

Vec2u::Vec2u(unsigned int x, unsigned int y) {
    elements[0] = x;
    elements[1] = y;
}

void Vec2u::add(const Vec2u& other) {
    elements[0] += other[0];
    elements[1] += other[1];
}

void Vec2u::subtract(const Vec2u& other) {
    elements[0] -= other[0];
    elements[1] -= other[1];
}

void Vec2u::multiply(const Vec2u& other) {
    elements[0] *= other[0];
    elements[1] *= other[1];
}

void Vec2u::divide(const Vec2u& other) {
    elements[0] /= other[0];
    elements[1] /= other[1];
}

const unsigned int& Vec2u::operator[](int index) const {
    assert(index >= 0 && index < 2);
    
    return elements[index];
}


unsigned int& Vec2u::operator[](int index) {
    assert(index >= 0 && index < 2);
    
    return elements[index];
}

void Vec2u::operator+=(const Vec2u& other) {
    add(other);
}

void Vec2u::operator-=(const Vec2u& other) {
    subtract(other);
}

void Vec2u::operator*=(const Vec2u& other) {
    multiply(other);
}

void Vec2u::operator/=(const Vec2u& other) {
    divide(other);
}

void Vec2u::operator=(const Vec2u& other) {
    x = other.x;
    y = other.y;
}

Vec2u operator+(Vec2u left, Vec2u right) {
    Vec2u result(left);
    result.add(right);
    return result;
}

Vec2u operator-(Vec2u left, Vec2u right) {
    Vec2u result(left);
    result.subtract(right);
    return result;
}

Vec2u operator*(Vec2u left, Vec2u right) {
    Vec2u result(left);
    result.multiply(right);
    return result;
}

Vec2u operator/(Vec2u left, Vec2u right) {
    Vec2u result(left);
    result.divide(right);
    return result;
}
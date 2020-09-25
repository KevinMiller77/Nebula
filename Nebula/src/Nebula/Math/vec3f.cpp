#include "vec3f.h"

#include <string.h>
#include <assert.h>

Vec3f::Vec3f(){
    memset(elements, 0, 3 * sizeof(float));
}

Vec3f::~Vec3f(){}

Vec3f::Vec3f(float x, float y, float z){
    elements[0] = x;
    elements[1] = y;
    elements[2] = z;
    
}

void Vec3f::add(const Vec3f& other) {
    elements[0] += other[0];
    elements[1] += other[1];
    elements[2] += other[2];
}

void Vec3f::subtract(const Vec3f& other) {
    elements[0] -= other[0];
    elements[1] -= other[1];
    elements[2] -= other[2];
}

void Vec3f::multiply(const Vec3f& other) {
    elements[0] *= other[0];
    elements[1] *= other[1];
    elements[2] *= other[2];
}

void Vec3f::divide(const Vec3f& other) {
    elements[0] /= other[0];
    elements[1] /= other[1];
    elements[2] /= other[2];
}

const float& Vec3f::operator[](int index) const {
    assert(index >= 0 && index < 3);
    
    return elements[index];
}

float& Vec3f::operator[](int index) {
    assert(index >= 0 && index < 3);
    
    return elements[index];
}

void Vec3f::operator+=(const Vec3f& other) {
    add(other);
}

void Vec3f::operator-=(const Vec3f& other) {
    subtract(other);
}

void Vec3f::operator*=(const Vec3f& other) {
    multiply(other);
}

void Vec3f::operator/=(const Vec3f& other) {
    divide(other);
}

bool Vec3f::operator==(const Vec3f& other)
{
    return (x == other.x && (y == other.y && z == other.z));
}


Vec3f operator+(Vec3f left, Vec3f right) {
    Vec3f result(left);
    result.add(right);
    return result;
}

Vec3f operator-(Vec3f left, Vec3f right) {
    Vec3f result(left);
    result.subtract(right);
    return result;
}

Vec3f operator*(Vec3f left, Vec3f right) {
    Vec3f result(left);
    result.multiply(right);
    return result;
}

Vec3f operator/(Vec3f left, Vec3f right) {
    Vec3f result(left);
    result.divide(right);
    return result;
}
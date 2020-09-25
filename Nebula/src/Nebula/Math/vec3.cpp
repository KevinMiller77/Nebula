#include "vec3.h"

#include <string.h>
#include <assert.h>

Vec3::Vec3(){
    memset(elements, 0, 3 * sizeof(float));
}

Vec3::~Vec3(){}

Vec3::Vec3(float x, float y, float z){
    elements[0] = x;
    elements[1] = y;
    elements[2] = z;
    
}

void Vec3::add(const Vec3& other) {
    elements[0] += other[0];
    elements[1] += other[1];
    elements[2] += other[2];
}

void Vec3::subtract(const Vec3& other) {
    elements[0] -= other[0];
    elements[1] -= other[1];
    elements[2] -= other[2];
}

void Vec3::multiply(const Vec3& other) {
    elements[0] *= other[0];
    elements[1] *= other[1];
    elements[2] *= other[2];
}

void Vec3::divide(const Vec3& other) {
    elements[0] /= other[0];
    elements[1] /= other[1];
    elements[2] /= other[2];
}

const float& Vec3::operator[](int index) const {
    assert(index >= 0 && index < 3);
    
    return elements[index];
}

float& Vec3::operator[](int index) {
    assert(index >= 0 && index < 3);
    
    return elements[index];
}

void Vec3::operator+=(const Vec3& other) {
    add(other);
}

void Vec3::operator-=(const Vec3& other) {
    subtract(other);
}

void Vec3::operator*=(const Vec3& other) {
    multiply(other);
}

void Vec3::operator/=(const Vec3& other) {
    divide(other);
}

Vec3 operator+(Vec3 left, Vec3 right) {
    Vec3 result(left);
    result.add(right);
    return result;
}

Vec3 operator-(Vec3 left, Vec3 right) {
    Vec3 result(left);
    result.subtract(right);
    return result;
}

Vec3 operator*(Vec3 left, Vec3 right) {
    Vec3 result(left);
    result.multiply(right);
    return result;
}

Vec3 operator/(Vec3 left, Vec3 right) {
    Vec3 result(left);
    result.divide(right);
    return result;
}
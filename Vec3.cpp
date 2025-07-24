#include "Vec3.h"
#include <iostream>
#include <cmath>

Vec3::Vec3() : x(0), y(0), z(0) {}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

float Vec3::operator*(const Vec3& v) const {
    return x * v.x + y * v.y + z * v.z;
}

Vec3 Vec3::operator^(const Vec3& v) const {
    return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

Vec3 Vec3::operator-(const Vec3& v) const {
    return Vec3(x - v.x, y - v.y, z - v.z);
}

Vec3 Vec3::operator+(const Vec3& v) const {
    return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3::operator*(const float& f) const {
    return Vec3(x * f, y * f, z * f);
}

Vec3 Vec3::normalize() const {
    float length = std::sqrt(x * x + y * y + z * z);
    return Vec3(x / length, y / length, z / length);
}

std::string Vec3::toString() const {
    return "x: " + std::to_string(x) +
           ", y: " + std::to_string(y) +
           ", z: " + std::to_string(z);
}

void Vec3::print() const {
    std::cout << "(" << x << ", " << y << ", " << z << ")\n";
}



#include "Vec3.h"
#include <iostream>

Vec3::Vec3() : x(0), y(0), z(0), w(1) {}

Vec3::Vec3(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

float Vec3::operator*(const Vec3& v) const {
    return x * v.x + y * v.y + z * v.z;
}

Vec3 Vec3::operator^(const Vec3& v) const {
    return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x, 1);
}

Vec3 Vec3::operator-(const Vec3& v) const {
    return Vec3(x - v.x, y - v.y, z - v.z, 1);
}

Vec3 Vec3::operator+(const Vec3& v) const {
    return Vec3(x + v.x, y + v.y, z + v.z, 1);
}

Vec3 Vec3::operator*(const float& f) const {
    return Vec3(x * f, y * f, z * f, 1);
}

void Vec3::print() const {
    std::cout << "(" << x << ", " << y << ", " << z << ", " << w << ")\n";
}

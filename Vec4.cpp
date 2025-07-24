#include "Vec3.h"
#include "Vec4.h"
#include <iostream>


Vec4::Vec4() : x(0), y(0), z(0), w(1) {}

Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

Vec4::Vec4(Vec3& v) : x(v.x), y(v.y), z(v.z), w(1) {}

void Vec4::print() const {
    std::cout << "(" << x << ", " << y << ", " << z << ", " << w << ")\n";
}
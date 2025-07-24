#pragma once
#include "Vec3.h"
#include <vector>
#include <iostream>

class Quaternion {
public:
    float w, x, y, z;

    // Constructors
    Quaternion(); // Identity
    Quaternion(float w, float x, float y, float z);
    Quaternion(float angle, const Vec3& axis); // Axis-angle
    Quaternion(const Vec3& euler); // Euler angles (optional)

    // Core operations
    Quaternion operator*(const Quaternion& other) const;
    Quaternion operator*(float scalar) const;

    Quaternion conjugate() const;
    Quaternion inverse() const;
    float norm() const;
    void normalize();

    // Conversion
    std::vector<std::vector<float>> toRotationMatrix() const;

    // Rotate a vector
    Vec3 rotate(const Vec3& v) const;

    void applyRotation(const Quaternion& q);

    std::string toString() const; 

    void print() const;
};


#pragma once
#include <iostream>

class Vec3 {
public:
    float x, y, z;

    Vec3(); // default constructor
    Vec3(float x, float y, float z); // parameterized constructor

    // Dot product
    float operator*(const Vec3& v) const;

    // Cross product
    Vec3 operator^(const Vec3& v) const;

    // Subtraction
    Vec3 operator-(const Vec3& v) const;

    // Addition
    Vec3 operator+(const Vec3& v) const;

    // Multiplication by scalar
    Vec3 operator*(const float& f) const;

    Vec3 normalize() const;

    std::string toString() const;

    void print() const;
};
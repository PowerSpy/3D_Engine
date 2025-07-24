#include "Quaternion.h"
#include <cmath>
#include <iostream>

constexpr float DEG2RAD = 3.14159265f / 180.0f;

// Default constructor (identity quaternion)
Quaternion::Quaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}

// Constructor from components
Quaternion::Quaternion(float w_, float x_, float y_, float z_)
    : w(w_), x(x_), y(y_), z(z_) {}

// Constructor from axis-angle (angle in radians, axis must be normalized)
Quaternion::Quaternion(float radian, const Vec3& axis) {
    float halfAngle = radian * 0.5f;
    float s = std::sin(halfAngle);
    w = std::cos(halfAngle);
    x = axis.x * s;
    y = axis.y * s;
    z = axis.z * s;
}

// Constructor from Euler angles (in radians, YXZ order: yaw (y), pitch (x), roll (z))
Quaternion::Quaternion(const Vec3& euler) {
    float yaw = euler.y;
    float pitch = euler.x;
    float roll = euler.z;

    float halfYaw   = yaw * 0.5f; // Y
    float halfPitch = pitch * 0.5f; // X
    float halfRoll  = roll * 0.5f; // Z

    float cy = std::cos(halfYaw);
    float sy = std::sin(halfYaw);
    float cp = std::cos(halfPitch);
    float sp = std::sin(halfPitch);
    float cr = std::cos(halfRoll);
    float sr = std::sin(halfRoll);

    // Apply in Y (yaw) * X (pitch) * Z (roll) order
    w = cy * cr * cp + sy * sr * sp;
    x = cy * sr * cp - sy * cr * sp;
    y = cy * cr * sp + sy * sr * cp;
    z = sy * cr * cp - cy * sr * sp;
}


// Quaternion multiplication (combines rotations)
Quaternion Quaternion::operator*(const Quaternion& other) const {
    return Quaternion(
        w * other.w - x * other.x - y * other.y - z * other.z,
        w * other.x + x * other.w + y * other.z - z * other.y,
        w * other.y - x * other.z + y * other.w + z * other.x,
        w * other.z + x * other.y - y * other.x + z * other.w
    );
}

// Scalar multiplication
Quaternion Quaternion::operator*(float scalar) const {
    return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar);
}

// Conjugate
Quaternion Quaternion::conjugate() const {
    return Quaternion(w, -x, -y, -z);
}

// Inverse (assumes normalized quaternion)
Quaternion Quaternion::inverse() const {
    float n = norm();
    if (n == 0.0f) return Quaternion();
    return conjugate() * (1.0f / n);
}

// Norm (magnitude)
float Quaternion::norm() const {
    return std::sqrt(w * w + x * x + y * y + z * z);
}

// Normalize in-place
void Quaternion::normalize() {
    float n = norm();
    if (n == 0.0f) return;
    w /= n;
    x /= n;
    y /= n;
    z /= n;
}

// Convert to 4x4 rotation matrix (column-major, OpenGL style)
std::vector<std::vector<float>> Quaternion::toRotationMatrix() const {
    float xx = x * x, yy = y * y, zz = z * z;
    float xy = x * y, xz = x * z, yz = y * z;
    float wx = w * x, wy = w * y, wz = w * z;

    std::vector<std::vector<float>> matrix(4, std::vector<float>(4, 0.0f));

    matrix[0][0] = 1.0f - 2.0f * (yy + zz);
    matrix[0][1] = 2.0f * (xy + wz);
    matrix[0][2] = 2.0f * (xz - wy);
    matrix[0][3] = 0.0f;

    matrix[1][0] = 2.0f * (xy - wz);
    matrix[1][1] = 1.0f - 2.0f * (xx + zz);
    matrix[1][2] = 2.0f * (yz + wx);
    matrix[1][3] = 0.0f;

    matrix[2][0] = 2.0f * (xz + wy);
    matrix[2][1] = 2.0f * (yz - wx);
    matrix[2][2] = 1.0f - 2.0f * (xx + yy);
    matrix[2][3] = 0.0f;


    return matrix;
}


// Rotate a vector by this quaternion
Vec3 Quaternion::rotate(const Vec3& v) const {
    Quaternion qv(0, v.x, v.y, v.z);
    Quaternion result = (*this) * qv * this->conjugate();
    return Vec3(result.x, result.y, result.z);
} 

void Quaternion::applyRotation(const Quaternion& q) {
    (*this) = (*this) * q;
}

std::string Quaternion::toString() const {
    return "x: " + std::to_string(x) +
           ", y: " + std::to_string(y) +
           ", z: " + std::to_string(z) +
           ", w: " + std::to_string(w);
}

void Quaternion::print() const {
    std::cout << "Quaternion: " << w << " " << x << " " << y << " " << z << std::endl;
}
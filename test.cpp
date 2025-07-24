#include "Quaternion.h"
#include "Vec3.h"
#include <iostream>
#include <cmath>


// Helper for printing vectors
void printVec(const std::string& label, const Vec3& v) {
    std::cout << label << ": (" << v.x << ", " << v.y << ", " << v.z << ")\n";
}

// Helper for printing quaternions
void printQuat(const std::string& label, const Quaternion& q) {
    std::cout << label << ": (w: " << q.w << ", x: " << q.x << ", y: " << q.y << ", z: " << q.z << ")\n";
}


int main() {
    // Define input vector — forward direction
    Vec3 forward(0, 0, -1);
    printVec("Original Forward", forward);

    // Set yaw rotation (90 degrees to the right)
    float degrees = 90.0f;
    float radians = degrees * M_PI / 180.0f;

    // Create a quaternion that rotates 90 degrees around Y axis
    Quaternion yawRotation(radians, Vec3(0.0f, 1, 0.0f));
    printQuat("Yaw Rotation Quaternion", yawRotation);

    // Rotate the forward vector
    Vec3 rotated = yawRotation.rotate(forward);
    printVec("Rotated Forward", rotated);

    // Try other vectors (e.g. right and up)
    Vec3 right(1, 0, 0);
    Vec3 up(0, 1, 0);
    printVec("Original Right", right);
    printVec("Original Up", up);

    Vec3 rotatedRight = yawRotation.rotate(right);
    Vec3 rotatedUp = yawRotation.rotate(up);

    printVec("Rotated Right", rotatedRight);
    printVec("Rotated Up", rotatedUp);

    return 0;
}

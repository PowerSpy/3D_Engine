#pragma once

#include "Vec3.h"
#include <vector>
#include "Quaternion.h"

class Camera {
public:
    Vec3 position;
    Quaternion rotation;

    Camera();                                    // Default constructor
    Camera(const Vec3& pos);                     // Constructor with position

    std::vector<std::vector<float>> getTranslationMatrix() const;
    std::vector<std::vector<float>> getRotationMatrix() const;
    std::vector<std::vector<float>> getViewMatrix() const;

    // Movement methods
    Vec3 getForwardVector() const;
    Vec3 getRightVector() const;
    Vec3 getUpVector() const;
    
    void moveForward(float distance);
    void moveRight(float distance);
    void moveUp(float distance);

    void print() const;
};
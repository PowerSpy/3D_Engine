#include "Camera.h"
#include <iostream>
#include <cmath>
#include "Quaternion.h"

Camera::Camera() : position(0.0f, 0.0f, 0.0f), rotation(1.0f, 0.0f, 0.0f, 0.0f) {}

Camera::Camera(const Vec3& pos) : position(pos), rotation(1.0f, 0.0f, 0.0f, 0.0f) {}

std::vector<std::vector<float>> Camera::getTranslationMatrix() const {
    std::vector<std::vector<float>> translationMatrix(4, std::vector<float>(4, 0.0f));
    translationMatrix[0][0] = 1.0f;
    translationMatrix[1][1] = 1.0f;
    translationMatrix[2][2] = 1.0f;
    translationMatrix[3][3] = 1.0f;

    translationMatrix[0][3] = -position.x;
    translationMatrix[1][3] = -position.y;
    translationMatrix[2][3] = -position.z;

    return translationMatrix;
}

std::vector<std::vector<float>> Camera::getRotationMatrix() const {
    return rotation.toRotationMatrix();
}

std::vector<std::vector<float>> Camera::getViewMatrix() const {
    std::vector<std::vector<float>> translationMatrix = getTranslationMatrix();
    std::vector<std::vector<float>> rotationMatrix = getRotationMatrix();

    std::vector<std::vector<float>> viewMatrix(4, std::vector<float>(4, 0.0f));

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            for (int i = 0; i < 4; i++) {
                viewMatrix[row][col] += rotationMatrix[row][i] * translationMatrix[i][col];
            }
        }
    }

    return viewMatrix;
}

Vec3 Camera::getForwardVector() const {
    return rotation.rotate(Vec3(0, 0, -1));
}

Vec3 Camera::getRightVector() const {
    return rotation.rotate(Vec3(1, 0, 0));
}

Vec3 Camera::getUpVector() const {
    return rotation.rotate(Vec3(0, 1, 0));
}

void Camera::moveForward(float distance) {
    Vec3 forward = getForwardVector();
    position.x += forward.x * distance;
    position.y += forward.y * distance;
    position.z += forward.z * distance;
}

void Camera::moveRight(float distance) {
    Vec3 right = getRightVector();
    position.x += right.x * distance;
    position.y += right.y * distance;
    position.z += right.z * distance;
}

void Camera::moveUp(float distance) {
    Vec3 up = getUpVector();
    position.x += up.x * distance;
    position.y += up.y * distance;
    position.z += up.z * distance;
}

void Camera::print() const {
    std::cout << "Camera Position: ";
    position.print();
    std::cout << "Camera Rotation: ";
    rotation.print();
}

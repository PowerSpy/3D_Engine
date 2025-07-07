#include "Face.h"
#include <cmath>

Face::Face(const std::vector<int>& indices) : vecIndices(indices) {}

void Face::draw(const std::vector<Vec3>& vertices, const std::vector<std::vector<float>>& projMatrix, SDL_Renderer* renderer, Camera& camera) const {
    std::vector<Vec3> projectedPoints;
    
    // Get camera view matrix
    std::vector<std::vector<float>> viewMatrix = camera.getViewMatrix();

    // Step 1: Check if all vertices are in front of the camera (in view space)
    float near = 0.1f; // Near plane distance
    bool allInFront = true;
    for (int idx : vecIndices) {
        Vec3 v = vertices[idx];
        // Transform to view space
        float viewZ = viewMatrix[2][0] * v.x + viewMatrix[2][1] * v.y + viewMatrix[2][2] * v.z + viewMatrix[2][3] * v.w;
        if (viewZ >= -near) { // For right-handed system, camera looks down -Z
            allInFront = false;
            break;
        }
    }
    if (!allInFront) return; // Skip drawing this face

    for (int idx : vecIndices) {
        Vec3 v = vertices[idx];

        // Apply view matrix (camera transformation)
        float viewX = viewMatrix[0][0] * v.x + viewMatrix[0][1] * v.y + viewMatrix[0][2] * v.z + viewMatrix[0][3] * v.w;
        float viewY = viewMatrix[1][0] * v.x + viewMatrix[1][1] * v.y + viewMatrix[1][2] * v.z + viewMatrix[1][3] * v.w;
        float viewZ = viewMatrix[2][0] * v.x + viewMatrix[2][1] * v.y + viewMatrix[2][2] * v.z + viewMatrix[2][3] * v.w;
        float viewW = viewMatrix[3][0] * v.x + viewMatrix[3][1] * v.y + viewMatrix[3][2] * v.z + viewMatrix[3][3] * v.w;

        // Apply projection matrix
        float x = projMatrix[0][0] * viewX + projMatrix[0][1] * viewY + projMatrix[0][2] * viewZ + projMatrix[0][3] * viewW;
        float y = projMatrix[1][0] * viewX + projMatrix[1][1] * viewY + projMatrix[1][2] * viewZ + projMatrix[1][3] * viewW;
        float z = projMatrix[2][0] * viewX + projMatrix[2][1] * viewY + projMatrix[2][2] * viewZ + projMatrix[2][3] * viewW;
        float w = projMatrix[3][0] * viewX + projMatrix[3][1] * viewY + projMatrix[3][2] * viewZ + projMatrix[3][3] * viewW;

        if (w != 0) {
            x /= w;
            y /= w;
            z /= w;
        }

        float screenX = (x + 1.0f) * 0.5f * 1600;
        float screenY = (1.0f - y) * 0.5f * 900;

        projectedPoints.emplace_back(screenX, screenY, z, 1.0f);
    }

    for (size_t i = 0; i < projectedPoints.size(); ++i) {
        const Vec3& p1 = projectedPoints[i];
        const Vec3& p2 = projectedPoints[(i + 1) % projectedPoints.size()];
        SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
    }
}

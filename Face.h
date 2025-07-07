#pragma once

#include <vector>
#include "Vec3.h"
#include <SDL2/SDL.h>
#include "Camera.h"

class Face {
public:
    std::vector<int> vecIndices;

    Face() = default;
    Face(const std::vector<int>& indices);

    void draw(const std::vector<Vec3>& vertices, const std::vector<std::vector<float>>& projMatrix, SDL_Renderer* renderer, Camera& camera) const;
};



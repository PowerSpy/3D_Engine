#pragma once

#include <vector>
#include <string>
#include "Vec3.h"
#include "Face.h"

class OBJParser {
public:
    static void loadOBJ(const std::string& filename, std::vector<Vec3>& vertices, std::vector<Face>& faces);
};

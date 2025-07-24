#include "OBJParser.h"
#include <fstream>
#include <sstream>
#include <iostream>

void OBJParser::loadOBJ(const std::string& filename, std::vector<Vec3>& vertices, std::vector<Face>& faces) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filename << "\n";
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            vertices.emplace_back(x, y, z);
        }
        else if (prefix == "f") {
            std::vector<int> indices;
            std::string vert;

            while (ss >> vert) {
                std::istringstream vs(vert);
                std::string indexStr;
                std::getline(vs, indexStr, '/');
                int index = std::stoi(indexStr) - 1;
                indices.push_back(index);
            }

            faces.emplace_back(indices);
        }
    }
    file.close();
}
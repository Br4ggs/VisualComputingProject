//
// Created by Kishan on 19/02/2025.
//

#ifndef MARCHING_CUBES_H
#define MARCHING_CUBES_H

#include <vector>
#include <glm/glm.hpp>

// Structure for a GridPoint (position + value)
struct GridPoint {
    glm::vec3 position;
    float value;
};

// Lookup Tables (declared as extern to be used in main.cpp)
extern const int edgeTable[256];
extern const int triTable[256][16];
extern const int EdgeVertexIndices[12][2];

// Function to apply Marching Cubes to a 3D scalar field
void marchingCubes(const std::vector<std::vector<std::vector<GridPoint>>>& grid, std::vector<glm::vec3>& vertices);

// Function to interpolate surface crossing on an edge
glm::vec3 interpolate(const GridPoint& p1, const GridPoint& p2);

#endif

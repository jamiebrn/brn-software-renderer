#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <rapidobj/rapidobj.hpp>

#include "Triangle.hpp"

namespace brn
{

struct Mesh
{
    std::vector<Triangle> triangles;
};

Mesh createCubeMesh(float height = 1.0f, float width = 1.0f, float depth = 1.0f, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255);
Mesh createPyramidSqMesh(float height = 1.0f, float width = 1.0f, float depth = 1.0f, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255);
Mesh createPlaneMesh(float height = 1.0f, float width = 1.0f, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255);

Mesh loadMeshFromFile(const std::string& filePath);

}
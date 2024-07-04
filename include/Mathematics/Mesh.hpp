#pragma once

#include <vector>

#include "Triangle.hpp"

namespace brn
{

struct Mesh
{
    std::vector<Triangle> triangles;
};

Mesh createCubeMesh(float height = 1.0f, float width = 1.0f, float depth = 1.0f, int r = 255, int g = 255, int b = 255);

}
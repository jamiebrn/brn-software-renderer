#pragma once

#include <vector>

#include "Triangle.hpp"

namespace brn
{

struct Mesh
{
    std::vector<Triangle> triangles;
};

Mesh createCubeMesh(float height = 1.0f, float width = 1.0f, float depth = 1.0f);

}
#pragma once

#include <vector>

#include "Triangle.hpp"

namespace brn
{

struct Mesh
{
    std::vector<Triangle> triangles;
};

Mesh createCubeMesh(float height, float width, float depth);

}
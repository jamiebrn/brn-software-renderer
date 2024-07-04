#pragma once

#include <array>
#include <queue>

#include "Mathematics/Triangle.hpp"
#include "Mathematics/Plane.hpp"

namespace brn
{

void clipTriangleOnPlane(Triangle triangle, Plane boundaryPlane, std::queue<brn::Triangle>& clippedTriangles);

void clipTriangles(std::queue<brn::Triangle>& triangles, Plane boundaryPlane);

}
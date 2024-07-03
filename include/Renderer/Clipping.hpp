#pragma once

#include <vector>

#include "Mathematics/Triangle.hpp"
#include "Mathematics/Plane.hpp"

namespace brn
{

std::vector<Triangle> clipTriangleOnPlane(Triangle triangle, Plane boundaryPlane);

void clipTriangles(std::vector<Triangle>& triangles, Plane boundaryPlane);

}
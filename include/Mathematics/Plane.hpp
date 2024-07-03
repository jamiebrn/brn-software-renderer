#pragma once

#include "Vector3.hpp"
#include "Vertex.hpp"

namespace brn
{

struct Plane
{
    float A, B, C, D;

    Plane() = default;
    Plane(Vector3 point, Vector3 normal);

    bool isVertexInFront(Vertex vertex);

    float calcIntersectionInterpolationValue(Vertex v1, Vertex v2);
};

}
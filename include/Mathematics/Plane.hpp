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

    bool isVertexInFront(const Vertex& vertex) const;

    float calcIntersectionInterpolationValue(const Vertex& v1, const Vertex& v2) const;
};

}
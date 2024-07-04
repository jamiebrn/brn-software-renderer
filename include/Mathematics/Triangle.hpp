#pragma once

#include <array>

#include "Vertex.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"

namespace brn
{

struct Triangle
{
    std::array<Vertex, 3> vertices;
    Triangle() = default;
    Triangle(Vertex a, Vertex b, Vertex c) : vertices{a, b, c} {}

    Vector3 calculateNormal();
    // bool isPointInTriangle(const Vector2i& point) const;
};

}
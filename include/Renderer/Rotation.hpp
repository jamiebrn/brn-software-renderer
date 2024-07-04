#pragma once

#include "math.h"

#include "Mathematics/Vertex.hpp"
#include "Mathematics/Vector3.hpp"

namespace brn
{

Vertex rotateVertex(const Vertex& vertex, const Vector3& rotation);

Vertex rotateVertexX(const Vertex& vertex, float angle);

Vertex rotateVertexY(const Vertex& vertex, float angle);

}
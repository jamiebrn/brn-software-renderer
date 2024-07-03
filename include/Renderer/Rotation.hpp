#pragma once

#include "math.h"

#include "Mathematics/Vertex.hpp"
#include "Mathematics/Vector3.hpp"

namespace brn
{

Vertex rotateVertex(Vertex vertex, Vector3 rotation);

Vertex rotateVertexX(Vertex vertex, float angle);

Vertex rotateVertexY(Vertex vertex, float angle);

}
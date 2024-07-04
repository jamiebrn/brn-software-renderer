#pragma once

#include <math.h>

#include "Mathematics/Vertex.hpp"

namespace brn
{

void vertexToClipSpace(Vertex& vertex, int screenWidth, int screenHeight, float fov, float near, float far);

}
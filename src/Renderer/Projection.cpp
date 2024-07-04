#include "Renderer/Projection.hpp"

void brn::vertexToClipSpace(Vertex& vertex, int screenWidth, int screenHeight, float fov, float near, float far)
{
    // Vertex clipSpaceVertex;
    vertex.w = vertex.z;
    vertex.x = vertex.x / (((float)screenWidth / (float)screenHeight) * tan(fov / 2.0));
    vertex.y = vertex.y / (tan(fov / 2.0));
    vertex.z = vertex.z * ((-far - near) / (near - far)) + (2 * far * near) / ((near - far));
    // return clipSpaceVertex;
}
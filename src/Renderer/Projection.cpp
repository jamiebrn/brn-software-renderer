#include "Renderer/Projection.hpp"

brn::Vertex brn::vertexToClipSpace(Vertex vertex, int screenWidth, int screenHeight, float fov, float near, float far)
{
    Vertex clipSpaceVertex;
    clipSpaceVertex.x = vertex.x / (((float)screenWidth / (float)screenHeight) * tan(fov / 2.0));
    clipSpaceVertex.y = vertex.y / (tan(fov / 2.0));
    clipSpaceVertex.z = vertex.z * ((-far - near) / (near - far)) + (2 * far * near) / ((near - far));
    clipSpaceVertex.w = vertex.z;
    return clipSpaceVertex;
}
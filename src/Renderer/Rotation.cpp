#include "Renderer/Rotation.hpp"

brn::Vertex brn::rotateVertex(const Vertex& vertex, const Vector3& rotation)
{
    Vertex rotatedVertex;
    rotatedVertex.x = (
        vertex.x * cos(rotation.y) * cos(rotation.z) +
        vertex.y * (sin(rotation.x) * sin(rotation.y) * cos(rotation.z) - cos(rotation.x) * sin(rotation.z)) +
        vertex.z * (cos(rotation.x) * sin(rotation.y) * cos(rotation.z) + sin(rotation.x) * sin(rotation.z))
    );
    rotatedVertex.y = (
        vertex.x * cos(rotation.y) * sin(rotation.z) +
        vertex.y * (sin(rotation.x) * sin(rotation.y) * sin(rotation.z) + cos(rotation.x) * cos(rotation.z)) +
        vertex.z * (cos(rotation.x) * sin(rotation.y) * sin(rotation.z) - sin(rotation.x) * cos(rotation.z))
    );
    rotatedVertex.z = (
        vertex.x * -sin(rotation.y) +
        vertex.y * sin(rotation.x) * cos(rotation.y) +
        vertex.z * cos(rotation.x) * cos(rotation.y)
    );
    return rotatedVertex;
}

brn::Vertex brn::rotateVertexX(const Vertex& vertex, float angle)
{
    Vertex rotatedVertex = vertex;
    rotatedVertex.y = vertex.y * cos(angle) - vertex.z * sin(angle);
    rotatedVertex.z = vertex.y * sin(angle) + vertex.z * cos(angle);
    return rotatedVertex;
}

brn::Vertex brn::rotateVertexY(const Vertex& vertex, float angle)
{
    Vertex rotatedVertex = vertex;
    rotatedVertex.x = vertex.x * cos(angle) + vertex.z * sin(angle);
    rotatedVertex.z = -vertex.x * sin(angle) + vertex.z * cos(angle);
    return rotatedVertex;
}
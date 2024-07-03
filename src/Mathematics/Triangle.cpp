#include "Mathematics/Triangle.hpp"

brn::Vector3 brn::Triangle::calculateNormal()
{
    Vector3 tangent = {
        vertices[0].x - vertices[1].x,
        vertices[0].y - vertices[1].y,
        vertices[0].z - vertices[1].z
    };
    Vector3 bitangent = {
        vertices[2].x - vertices[1].x,
        vertices[2].y - vertices[1].y,
        vertices[2].z - vertices[1].z
    };
    return Vector3{
        tangent.y * bitangent.z - tangent.z * bitangent.y,
        tangent.z * bitangent.x - tangent.x * bitangent.z,
        tangent.x * bitangent.y - tangent.y * bitangent.x
    };
}

bool brn::Triangle::isPointInTriangle(Vector2i point)
{
    for (int i = 0; i < 3; i++)
    {
        Vector2i edgeVector = {(int)vertices[(i + 1) % 3].x - (int)vertices[i].x, (int)vertices[(i + 1) % 3].y - (int)vertices[i].y};
        Vector2i pointVector = {point.x - (int)vertices[i].x, point.y - (int)vertices[i].y};

        int crossProduct = edgeVector.x * pointVector.y - pointVector.x * edgeVector.y;
        if (crossProduct < 0)
            return false;
    }
    return true;
}
#include "Mathematics/Plane.hpp"

brn::Plane::Plane(Vector3 point, Vector3 normal)
{
    A = normal.x;
    B = normal.y;
    C = normal.z;

    float dotProduct = point.x * normal.x + point.y * normal.y + point.z * normal.z;
    D = -dotProduct;
}

bool brn::Plane::isVertexInFront(const Vertex& vertex) const
{
    return A * vertex.x + B * vertex.y + C * vertex.z + D * vertex.w >= 0;
}

float brn::Plane::calcIntersectionInterpolationValue(const Vertex& v1, const Vertex& v2) const
{
    return -(v1.x * A + v1.y * B + v1.z * C + D * v1.w) / (A * (v2.x - v1.x) + B * (v2.y - v1.y) + C * (v2.z - v1.z) + D * (v2.w - v1.w));
}
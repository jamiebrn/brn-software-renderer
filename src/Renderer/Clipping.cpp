#include "Renderer/Clipping.hpp"

void brn::clipTriangleOnPlane(const Triangle& triangle, const Plane& boundaryPlane, std::queue<brn::Triangle>& clippedTriangles)
{
    std::array<Vertex, 5> clippedVertices;
    int clippedVertexCount = 0;

    // Clip all triangle vertices
    for (int i = 0; i < 3; i++)
    {
        Vertex v1 = triangle.vertices[i];
        Vertex v2 = triangle.vertices[(i + 1) % 3];

        bool v1InFront = boundaryPlane.isVertexInFront(v1);
        bool v2InFront = boundaryPlane.isVertexInFront(v2);

        if (v1InFront)
        {
            clippedVertices[clippedVertexCount] = v1;
            clippedVertexCount++;
        }

        if (v1InFront != v2InFront)
        {
            float interpolate = boundaryPlane.calcIntersectionInterpolationValue(v1, v2);
            Vertex intersection = v2;
            intersection.x = v1.x + interpolate * (v2.x - v1.x);
            intersection.y = v1.y + interpolate * (v2.y - v1.y);
            intersection.z = v1.z + interpolate * (v2.z - v1.z);
            intersection.w = v1.w + interpolate * (v2.w - v1.w);
            intersection.r = v1.r + interpolate * (v2.r - v1.r);
            intersection.g = v1.g + interpolate * (v2.g - v1.g);
            intersection.b = v1.b + interpolate * (v2.b - v1.b);
            intersection.u = v1.u + interpolate * (v2.u - v1.u);
            intersection.v = v1.v + interpolate * (v2.v - v1.v);
            clippedVertices[clippedVertexCount] = intersection;
            clippedVertexCount++;
        }
    }

    // Do not construct triangles if not enough vertices
    if (clippedVertexCount < 3)
        return;

    // Construct triangles from clipped vertices
    for (int i = 0; i < clippedVertexCount - 2; i++)
    {
        Triangle clippedTriangle;
        clippedTriangle.vertices[0] = clippedVertices[0];
        clippedTriangle.vertices[1] = clippedVertices[i + 1];
        clippedTriangle.vertices[2] = clippedVertices[i + 2];
        clippedTriangles.push(clippedTriangle);
    }
}

void brn::clipTriangles(std::queue<brn::Triangle>& triangles, const Plane& boundaryPlane)
{
    // std::vector<Triangle> clippedTriangles;
    int queueSize = triangles.size();
    for (int i = 0; i < queueSize; i++)
    {
        clipTriangleOnPlane(triangles.front(), boundaryPlane, triangles);
        triangles.pop();
    }
    // for (Triangle& tri : triangles)
    // {
    //     std::vector<Triangle> toAdd = clipTriangleOnPlane(tri, boundaryPlane);
    //     clippedTriangles.insert(clippedTriangles.end(), toAdd.begin(), toAdd.end());
    // }
    // triangles = clippedTriangles;
}
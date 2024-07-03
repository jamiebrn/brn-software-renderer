#include "Renderer/Clipping.hpp"

std::vector<brn::Triangle> brn::clipTriangleOnPlane(Triangle triangle, Plane boundaryPlane)
{
    std::vector<Vertex> clippedVertices;

    // Clip all triangle vertices
    for (int i = 0; i < 3; i++)
    {
        Vertex v1 = triangle.vertices[i];
        Vertex v2 = triangle.vertices[(i + 1) % 3];

        bool v1InFront = boundaryPlane.isVertexInFront(v1);
        bool v2InFront = boundaryPlane.isVertexInFront(v2);

        if (v1InFront) clippedVertices.push_back(v1);

        if (v1InFront != v2InFront)
        {
            float interpolate = boundaryPlane.calcIntersectionInterpolationValue(v1, v2);
            Vertex intersection;
            intersection.x = v1.x + interpolate * (v2.x - v1.x);
            intersection.y = v1.y + interpolate * (v2.y - v1.y);
            intersection.z = v1.z + interpolate * (v2.z - v1.z);
            intersection.w = v1.w + interpolate * (v2.w - v1.w);
            clippedVertices.push_back(intersection);
        }
    }

    // Construct triangles from clipped vertices
    std::vector<Triangle> clippedTriangles;
    if (clippedVertices.size() > 0)
    {
        for (int i = 0; i < clippedVertices.size() - 2; i++)
        {
            Triangle clippedTriangle;
            clippedTriangle.vertices[0] = clippedVertices[0];
            clippedTriangle.vertices[1] = clippedVertices[i + 1];
            clippedTriangle.vertices[2] = clippedVertices[i + 2];
            clippedTriangles.push_back(clippedTriangle);
        }
    }

    return clippedTriangles;
}

void brn::clipTriangles(std::vector<Triangle>& triangles, Plane boundaryPlane)
{
    std::vector<Triangle> clippedTriangles;
    for (Triangle& tri : triangles)
    {
        std::vector<Triangle> toAdd = clipTriangleOnPlane(tri, boundaryPlane);
        clippedTriangles.insert(clippedTriangles.end(), toAdd.begin(), toAdd.end());
    }
    triangles = clippedTriangles;
}
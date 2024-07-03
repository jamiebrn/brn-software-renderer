#pragma once

namespace brn
{

struct Vertex
{
    float x, y, z, w = 1.0;
    Vertex() = default;
    Vertex(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    inline Vertex operator*(float b)
    {
        Vertex newVertex;
        newVertex.x = x * b;
        newVertex.y = y * b;
        newVertex.z = z * b;
        return newVertex;
    }
};

}

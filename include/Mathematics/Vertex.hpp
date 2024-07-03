#pragma once

namespace brn
{

struct Vertex
{
    float x, y, z, w = 1.0;
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

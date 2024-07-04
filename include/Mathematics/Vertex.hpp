#pragma once

#include <cstdint>

namespace brn
{

struct Vertex
{
    float x, y, z, w = 1.0;
    uint8_t r = 255, g = 255, b = 255;
    Vertex() = default;
    Vertex(float _x, float _y, float _z, uint8_t _r = 255, uint8_t _g = 255, uint8_t _b = 255) : x(_x), y(_y), z(_z), r(_r), g(_g), b(_b) {}
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

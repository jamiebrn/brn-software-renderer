#include "Mathematics/Mesh.hpp"

brn::Mesh brn::createCubeMesh(float height, float width, float depth)
{
    Mesh cubeMesh = {{
        {{-width / 2.0f, height / 2.0f, depth / 2.0f},     {width / 2.0f, height / 2.0f, depth / 2.0f},       {-width / 2.0f, -height / 2.0f, depth / 2.0f}},
        {{width / 2.0f, height / 2.0f, depth / 2.0f},      {width / 2.0f, -height / 2.0f, depth / 2.0f},      {-width / 2.0f, -height / 2.0f, depth / 2.0f}},    // front
        {{width / 2.0f, height / 2.0f, -depth / 2.0f},     {-width / 2.0f, height / 2.0f, -depth / 2.0f},     {width / 2.0f, -height / 2.0f, -depth / 2.0f}},
        {{-width / 2.0f, height / 2.0f, -depth / 2.0f},    {-width / 2.0f, -height / 2.0f, -depth / 2.0f},    {width / 2.0f, -height / 2.0f, -depth / 2.0f}},    // back
        {{width / 2.0f, height / 2.0f, depth / 2.0f},      {width / 2.0f, height / 2.0f, -depth / 2.0f},       {width / 2.0f, -height / 2.0f, depth / 2.0f}},
        {{width / 2.0f, height / 2.0f, -depth / 2.0f},      {width / 2.0f, -height / 2.0f, -depth / 2.0f},    {width / 2.0f, -height / 2.0f, depth / 2.0f}},     // right
        {{-width / 2.0f, height / 2.0f, -depth / 2.0f},    {-width / 2.0f, height / 2.0f, depth / 2.0f},      {-width / 2.0f, -height / 2.0f, -depth / 2.0f}},
        {{-width / 2.0f, height / 2.0f, depth / 2.0f},     {-width / 2.0f, -height / 2.0f, depth / 2.0f},     {-width / 2.0f, -height / 2.0f, -depth / 2.0f}},   // left
        {{-width / 2.0f, height / 2.0f, -depth / 2.0f},    {width / 2.0f, height / 2.0f, -depth / 2.0f},      {-width / 2.0f, height / 2.0f, depth / 2.0f}},
        {{width / 2.0f, height / 2.0f, -depth / 2.0f},     {width / 2.0f, height / 2.0f, depth / 2.0f},       {-width / 2.0f, height / 2.0f, depth / 2.0f}},     // top
        {{-width / 2.0f, -height / 2.0f, depth / 2.0f},    {width / 2.0f, -height / 2.0f, depth / 2.0f},      {-width / 2.0f, -height / 2.0f, -depth / 2.0f}},
        {{width / 2.0f, -height / 2.0f, depth / 2.0f},     {width / 2.0f, -height / 2.0f, -depth / 2.0f},     {-width / 2.0f, -height / 2.0f, -depth / 2.0f}}    // bottom
    }};

    return cubeMesh;
}
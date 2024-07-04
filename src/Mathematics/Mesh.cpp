#include "Mathematics/Mesh.hpp"

brn::Mesh brn::createCubeMesh(float height, float width, float depth, uint8_t r, uint8_t g, uint8_t b)
{
    Mesh cubeMesh = {{
        {{-width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b},
        {width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b},
        {-width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b}},
        {{width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b},
        {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b},
        {-width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b}},    // front
        {{width / 2.0f, height / 2.0f, -depth / 2.0f, r, g, b},
        {-width / 2.0f, height / 2.0f, -depth / 2.0f, r, g, b},
        {width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b}},
        {{-width / 2.0f, height / 2.0f, -depth / 2.0f, r, g, b},
        {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b},
        {width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b}},    // back
        {{width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b},
        {width / 2.0f, height / 2.0f, -depth / 2.0f, r, g, b},
        {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b}},
        {{width / 2.0f, height / 2.0f, -depth / 2.0f, r, g, b},
        {width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b},
        {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b}},     // right
        {{-width / 2.0f, height / 2.0f, -depth / 2.0f, r, g, b},
        {-width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b},
        {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b}},
        {{-width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b},
        {-width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b},
        {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b}},   // left
        {{-width / 2.0f, height / 2.0f, -depth / 2.0f, r, g, b},
        {width / 2.0f, height / 2.0f, -depth / 2.0f, r, g, b},
        {-width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b}},
        {{width / 2.0f, height / 2.0f, -depth / 2.0f, r, g, b},
        {width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b},
        {-width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b}},     // top
        {{-width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b},
        {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b},
        {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b}},
        {{width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b},
        {width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b},
        {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b}}    // bottom
    }};

    return cubeMesh;
}
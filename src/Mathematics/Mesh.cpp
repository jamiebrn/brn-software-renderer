#include "Mathematics/Mesh.hpp"

brn::Mesh brn::createCubeMesh(float height, float width, float depth, uint8_t r, uint8_t g, uint8_t b)
{
    Mesh cubeMesh = {{
        {{-width / 2.0f, height / 2.0f, depth / 2.0f, r / 2.0, g, b},
        {width / 2.0f, height / 2.0f, depth / 2.0f, r, g / 2.0, b},
        {-width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b / 2.0}},
        {{width / 2.0f, height / 2.0f, depth / 2.0f, r / 2.0, g, b},
        {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g / 2.0, b},
        {-width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b / 2.0}},    // front
        {{width / 2.0f, height / 2.0f, -depth / 2.0f, r / 2.0, g, b},
        {-width / 2.0f, height / 2.0f, -depth / 2.0f, r, g / 2.0, b},
        {width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b / 2.0}},
        {{-width / 2.0f, height / 2.0f, -depth / 2.0f, r / 2.0, g, b},
        {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g / 2.0, b},
        {width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b / 2.0}},    // back
        {{width / 2.0f, height / 2.0f, depth / 2.0f, r / 2.0, g, b},
        {width / 2.0f, height / 2.0f, -depth / 2.0f, r, g / 2.0, b},
        {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b / 2.0}},
        {{width / 2.0f, height / 2.0f, -depth / 2.0f, r / 2.0, g, b},
        {width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g / 2.0, b},
        {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b / 2.0}},     // right
        {{-width / 2.0f, height / 2.0f, -depth / 2.0f, r / 2.0, g, b},
        {-width / 2.0f, height / 2.0f, depth / 2.0f, r, g / 2.0, b},
        {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b / 2.0}},
        {{-width / 2.0f, height / 2.0f, depth / 2.0f, r / 2.0, g, b},
        {-width / 2.0f, -height / 2.0f, depth / 2.0f, r, g / 2.0, b},
        {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b / 2.0}},   // left
        {{-width / 2.0f, height / 2.0f, -depth / 2.0f, r / 2.0, g, b},
        {width / 2.0f, height / 2.0f, -depth / 2.0f, r, g / 2.0, b},
        {-width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b / 2.0}},
        {{width / 2.0f, height / 2.0f, -depth / 2.0f, r / 2.0, g, b},
        {width / 2.0f, height / 2.0f, depth / 2.0f, r, g / 2.0, b},
        {-width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b / 2.0}},     // top
        {{-width / 2.0f, -height / 2.0f, depth / 2.0f, r / 2.0, g, b},
        {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g / 2.0, b},
        {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b / 2.0}},
        {{width / 2.0f, -height / 2.0f, depth / 2.0f, r / 2.0, g, b},
        {width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g / 2.0, b},
        {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b / 2.0}}    // bottom
    }};

    return cubeMesh;
}

brn::Mesh brn::createPyramidSqMesh(float height, float width, float depth, uint8_t r, uint8_t g, uint8_t b)
{
    Mesh pyramidSqMesh = {{
        {
            {-width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b},
            {0, height / 2.0f, 0, r, g, b},
            {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b}
        },
        {
            {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b},
            {0, height / 2.0f, 0, r, g, b},
            {width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b}
        },
        {
            {width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b},
            {0, height / 2.0f, 0, r, g, b},
            {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b}
        },
        {
            {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b},
            {0, height / 2.0f, 0, r, g, b},
            {-width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b}
        },
        {
            {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b},
            {-width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b},
            {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b}
        },
        {
            {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b},
            {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b},
            {width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b}
        }
    }};

    return pyramidSqMesh;
}
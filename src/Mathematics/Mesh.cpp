#include "Mathematics/Mesh.hpp"

brn::Mesh brn::createCubeMesh(float height, float width, float depth, uint8_t r, uint8_t g, uint8_t b)
{
    Mesh cubeMesh = {{
        {{-width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b, 0, 0},
        {width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b, 1, 0},
        {-width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b, 0, 1}},
        {{width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b, 1, 0},
        {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b, 1, 1},
        {-width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b, 0, 1}},    // front
        {{width / 2.0f, height / 2.0f, -depth / 2.0f, r, g, b, 0, 0},
        {-width / 2.0f, height / 2.0f, -depth / 2.0f, r, g, b, 1, 0},
        {width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b, 0, 1}},
        {{-width / 2.0f, height / 2.0f, -depth / 2.0f, r, g, b, 1, 0},
        {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b, 1, 1},
        {width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b, 0, 1}},    // back
        {{width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b, 0, 0},
        {width / 2.0f, height / 2.0f, -depth / 2.0f, r, g, b, 1, 0},
        {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b, 0, 1}},
        {{width / 2.0f, height / 2.0f, -depth / 2.0f, r, g, b, 1, 0},
        {width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b, 1, 1},
        {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b, 0, 1}},     // right
        {{-width / 2.0f, height / 2.0f, -depth / 2.0f, r, g, b, 0, 0},
        {-width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b, 1, 0},
        {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b, 0, 1}},
        {{-width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b, 1, 0},
        {-width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b, 1, 1},
        {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b, 0, 1}},   // left
        {{-width / 2.0f, height / 2.0f, -depth / 2.0f, r, g, b, 0, 0},
        {width / 2.0f, height / 2.0f, -depth / 2.0f, r, g, b, 1, 0},
        {-width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b, 0, 1}},
        {{width / 2.0f, height / 2.0f, -depth / 2.0f, r, g, b, 1, 0},
        {width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b, 1, 1},
        {-width / 2.0f, height / 2.0f, depth / 2.0f, r, g, b, 0, 1}},     // top
        {{-width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b, 0, 0},
        {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b, 1, 0},
        {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b, 0, 1}},
        {{width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b, 1, 0,},
        {width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b, 1, 1},
        {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b, 0, 1}}    // bottom
    }};

    return cubeMesh;
}

brn::Mesh brn::createPyramidSqMesh(float height, float width, float depth, uint8_t r, uint8_t g, uint8_t b)
{
    Mesh pyramidSqMesh = {{
        {
            {-width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b, 0, 1},
            {0, height / 2.0f, 0, r, g, b, 0.5, 0},
            {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b, 1, 1}
        },
        {
            {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b, 0, 1},
            {0, height / 2.0f, 0, r, g, b, 0.5, 0},
            {width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b, 1, 1}
        },
        {
            {width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b, 0, 1},
            {0, height / 2.0f, 0, r, g, b, 0.5, 0},
            {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b, 1, 1}
        },
        {
            {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b, 0, 1},
            {0, height / 2.0f, 0, r, g, b, 0.5, 0},
            {-width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b, 1, 1}
        },
        {
            {-width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b, 0, 0},
            {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b, 1, 0},
            {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b, 0, 1}
        },
        {
            {width / 2.0f, -height / 2.0f, depth / 2.0f, r, g, b, 1, 0,},
            {width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b, 1, 1},
            {-width / 2.0f, -height / 2.0f, -depth / 2.0f, r, g, b, 0, 1}
        }
    }};

    return pyramidSqMesh;
}

brn::Mesh brn::createPlaneMesh(float height, float width, uint8_t r, uint8_t g, uint8_t b)
{
    Mesh planeMesh = {{
        {{-width / 2.0f, height / 2.0f, 0, r, g, b, 0, 0},
        {width / 2.0f, height / 2.0f, 0, r, g, b, width, 0},
        {-width / 2.0f, -height / 2.0f, 0, r, g, b, 0, height}},
        {{width / 2.0f, height / 2.0f, 0, r, g, b, width, 0},
        {width / 2.0f, -height / 2.0f, 0, r, g, b, width, height},
        {-width / 2.0f, -height / 2.0f, 0, r, g, b, 0, height}}
    }};

    return planeMesh;
}
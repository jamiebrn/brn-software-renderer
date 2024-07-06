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

brn::Mesh brn::loadMeshFromFile(const std::string& filePath)
{
    Mesh mesh;
    rapidobj::Result result = rapidobj::ParseFile(std::filesystem::path(filePath));
    rapidobj::Triangulate(result);
    int mIndex = 0;
    for (int i = 0; i < result.shapes[0].mesh.indices.size(); i+=3)
    {
        float u0 = 0, v0 = 0;
        float u1 = 0, v1 = 0;
        float u2 = 0, v2 = 0;
        if (result.attributes.texcoords.size() > 0)
        {
            u0 = result.attributes.texcoords[(result.shapes[0].mesh.indices[i].texcoord_index) * 2];
            v0 = -result.attributes.texcoords[(result.shapes[0].mesh.indices[i].texcoord_index) * 2 + 1];
            u1 = result.attributes.texcoords[(result.shapes[0].mesh.indices[i + 1].texcoord_index) * 2];
            v1 = -result.attributes.texcoords[(result.shapes[0].mesh.indices[i + 1].texcoord_index) * 2 + 1];
            u2 = result.attributes.texcoords[(result.shapes[0].mesh.indices[i + 2].texcoord_index) * 2];
            v2 = -result.attributes.texcoords[(result.shapes[0].mesh.indices[i + 2].texcoord_index) * 2 + 1];
        }
        mesh.triangles.push_back({
            {result.attributes.positions[(result.shapes[0].mesh.indices[i].position_index) * 3],
            result.attributes.positions[(result.shapes[0].mesh.indices[i].position_index) * 3 + 1],
            result.attributes.positions[(result.shapes[0].mesh.indices[i].position_index) * 3 + 2],
            (uint8_t)(result.materials[result.shapes[0].mesh.material_ids[mIndex]].diffuse[0] * 255),
            (uint8_t)(result.materials[result.shapes[0].mesh.material_ids[mIndex]].diffuse[1] * 255),
            (uint8_t)(result.materials[result.shapes[0].mesh.material_ids[mIndex]].diffuse[2] * 255),
            u0,
            v0},
            {result.attributes.positions[(result.shapes[0].mesh.indices[i + 1].position_index) * 3],
            result.attributes.positions[(result.shapes[0].mesh.indices[i + 1].position_index) * 3 + 1],
            result.attributes.positions[(result.shapes[0].mesh.indices[i + 1].position_index) * 3 + 2],
            (uint8_t)(result.materials[result.shapes[0].mesh.material_ids[mIndex]].diffuse[0] * 255),
            (uint8_t)(result.materials[result.shapes[0].mesh.material_ids[mIndex]].diffuse[1] * 255),
            (uint8_t)(result.materials[result.shapes[0].mesh.material_ids[mIndex]].diffuse[2] * 255),
            u1,
            v1},
            {result.attributes.positions[(result.shapes[0].mesh.indices[i + 2].position_index) * 3],
            result.attributes.positions[(result.shapes[0].mesh.indices[i + 2].position_index) * 3 + 1],
            result.attributes.positions[(result.shapes[0].mesh.indices[i + 2].position_index) * 3 + 2],
            (uint8_t)(result.materials[result.shapes[0].mesh.material_ids[mIndex]].diffuse[0] * 255),
            (uint8_t)(result.materials[result.shapes[0].mesh.material_ids[mIndex]].diffuse[1] * 255),
            (uint8_t)(result.materials[result.shapes[0].mesh.material_ids[mIndex]].diffuse[2] * 255),
            u2,
            v2}
        });
        mIndex++;
    }
    return mesh;
}
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <memory>
#include <math.h>
#include <iostream>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCREEN_RENDER_SCALE 1

struct Vertex {
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

struct Triangle {
    std::array<Vertex, 3> vertices;
    Triangle() = default;
    Triangle(Vertex a, Vertex b, Vertex c) : vertices{a, b, c} {}
};

struct Plane {
    float A, B, C, D;
};

Plane makePlane(sf::Vector3f point, sf::Vector3f normal)
{
    Plane plane;
    plane.A = normal.x;
    plane.B = normal.y;
    plane.C = normal.z;

    float dotProduct = point.x * normal.x + point.y * normal.y + point.z * normal.z;
    plane.D = -dotProduct;

    return plane;
}

bool isVertexInFrontPlane(Vertex vertex, Plane plane)
{
    return plane.A * vertex.x + plane.B * vertex.y + plane.C * vertex.z + plane.D >= 0;
}

float planeIntersectionInterpolationValue(Vertex v1, Vertex v2, Plane plane)
{
    return -(v1.x * plane.A + v1.y * plane.B + v1.z * plane.C + plane.D) / (plane.A * (v2.x - v1.x) + plane.B * (v2.y - v1.y) + plane.C * (v2.z - v1.z));
}

std::vector<Triangle> clipTriangleToView(Triangle triangle, Plane boundaryPlane)
{
    std::vector<Vertex> clippedVertices;

    // Clip all triangle vertices
    for (int i = 0; i < 3; i++)
    {
        Vertex v1 = triangle.vertices[i];
        Vertex v2 = triangle.vertices[(i + 1) % 3];

        Vertex intersection;
        float interpolation = planeIntersectionInterpolationValue(v1, v2, boundaryPlane);
        intersection.x = (v2.x - v1.x) * interpolation + v1.x;
        intersection.y = (v2.y - v1.y) * interpolation + v1.y;
        intersection.z = (v2.z - v1.z) * interpolation + v1.z;
        intersection.w = (v2.w - v1.w) * interpolation + v1.w;

        if (isVertexInFrontPlane(v2, boundaryPlane))
        {
            if (!isVertexInFrontPlane(v1, boundaryPlane))
            {
                clippedVertices.push_back(intersection);
            }
            clippedVertices.push_back(v2);
        }
        else if (isVertexInFrontPlane(v1, boundaryPlane))
        {
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

Vertex vertexToClipSpace(Vertex vertex, sf::Vector2u screenSize, float fov, float near, float far)
{
    Vertex clipSpaceVertex;
    clipSpaceVertex.x = vertex.x / (((float)screenSize.x / (float)screenSize.y) * tan(fov / 2.0));
    clipSpaceVertex.y = vertex.y / (tan(fov / 2.0));
    clipSpaceVertex.z = vertex.z * ((-far - near) / (near - far)) + (2 * far * near) / ((near - far));
    clipSpaceVertex.w = vertex.z;
    return clipSpaceVertex;
}

Vertex rotateVertex(Vertex vertex, sf::Vector3f rotation)
{
    Vertex rotatedVertex;
    rotatedVertex.x = (
        vertex.x * cos(rotation.y) * cos(rotation.z) +
        vertex.y * (sin(rotation.x) * sin(rotation.y) * cos(rotation.z) - cos(rotation.x) * sin(rotation.z)) +
        vertex.z * (cos(rotation.x) * sin(rotation.y) * cos(rotation.z) + sin(rotation.x) * sin(rotation.z))
    );
    rotatedVertex.y = (
        vertex.x * cos(rotation.y) * sin(rotation.z) +
        vertex.y * (sin(rotation.x) * sin(rotation.y) * sin(rotation.z) + cos(rotation.x) * cos(rotation.z)) +
        vertex.z * (cos(rotation.x) * sin(rotation.y) * sin(rotation.z) - sin(rotation.x) * cos(rotation.z))
    );
    rotatedVertex.z = (
        vertex.x * -sin(rotation.y) +
        vertex.y * sin(rotation.x) * cos(rotation.y) +
        vertex.z * cos(rotation.x) * cos(rotation.y)
    );
    return rotatedVertex;
}

Vertex rotateVertexX(Vertex vertex, float angle)
{
    Vertex rotatedVertex = vertex;
    rotatedVertex.y = vertex.y * cos(angle) - vertex.z * sin(angle);
    rotatedVertex.z = vertex.y * sin(angle) + vertex.z * cos(angle);
    return rotatedVertex;
}

Vertex rotateVertexY(Vertex vertex, float angle)
{
    Vertex rotatedVertex = vertex;
    rotatedVertex.x = vertex.x * cos(angle) + vertex.z * sin(angle);
    rotatedVertex.z = -vertex.x * sin(angle) + vertex.z * cos(angle);
    return rotatedVertex;
}

sf::Vector3f calculateTriNormal(Triangle tri)
{
    sf::Vector3f tangent = {
        tri.vertices[0].x - tri.vertices[1].x,
        tri.vertices[0].y - tri.vertices[1].y,
        tri.vertices[0].z - tri.vertices[1].z
    };
    sf::Vector3f bitangent = {
        tri.vertices[2].x - tri.vertices[1].x,
        tri.vertices[2].y - tri.vertices[1].y,
        tri.vertices[2].z - tri.vertices[1].z
    };
    return sf::Vector3f(
        tangent.y * bitangent.z - tangent.z * bitangent.y,
        tangent.z * bitangent.x - tangent.x * bitangent.z,
        tangent.x * bitangent.y - tangent.y * bitangent.x
    );
}

void clearPixelBuffer(std::array<sf::Uint8, SCREEN_WIDTH * SCREEN_HEIGHT * 4>* pixelBuffer)
{
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT * 4; i += 4)
    {
        (*pixelBuffer)[i] = 0;
        (*pixelBuffer)[i + 1] = 0;
        (*pixelBuffer)[i + 2] = 0;
        (*pixelBuffer)[i + 3] = 255;
    }
}

void drawLineToPixelBuffer(sf::Vector2u a, sf::Vector2u b, std::array<sf::Uint8, SCREEN_WIDTH * SCREEN_HEIGHT * 4>* pixelBuffer, int cr = 255, int cg = 255, int cb = 255)
{
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    int steps = std::max(std::abs(dx), std::abs(dy));
    float xInc = (float)dx / (float)steps;
    float yInc = (float)dy / (float)steps;
    float x = a.x, y = a.y;
    for (int i = 0; i <= steps; i++)
    {
        if (x > 0 && x < SCREEN_WIDTH - 1 && y > 0 && y < SCREEN_HEIGHT - 1)
        {
            // Draw pixel to buffer
            int pixel = (int)(round(x) * 4) + (int)(round(y) * SCREEN_WIDTH * 4);
            (*pixelBuffer)[pixel] = cr;
            (*pixelBuffer)[pixel + 1] = cg;
            (*pixelBuffer)[pixel + 2] = cb;
        }
        x += xInc;
        y += yInc;
    }
}

void drawTriangleToPixelBuffer(Triangle tri, std::array<sf::Uint8, SCREEN_WIDTH * SCREEN_HEIGHT * 4>* pixelBuffer, int cr = 255, int cg = 255, int cb = 255)
{
    // Draw lines of triangle
    for (int i = 0; i < 3; i++)
    {
        Vertex vertexOne = tri.vertices[i];
        Vertex vertexTwo = tri.vertices[(i + 1) % 3];

        // if (vertexOne.z < -1 || vertexOne.z > 1 || vertexTwo.z < -1 || vertexTwo.z > 1)
            // continue;
        
        drawLineToPixelBuffer({(unsigned int)vertexOne.x, (unsigned int)vertexOne.y}, {(unsigned int)vertexTwo.x, (unsigned int)vertexTwo.y}, pixelBuffer, cr, cg, cb);
    }
}

bool isPointInTriangle(Triangle tri, sf::Vector2i point)
{
    for (int i = 0; i < 3; i++)
    {
        sf::Vector2i edgeVector = {(int)tri.vertices[(i + 1) % 3].x - (int)tri.vertices[i].x, (int)tri.vertices[(i + 1) % 3].y - (int)tri.vertices[i].y};
        sf::Vector2i pointVector = {point.x - (int)tri.vertices[i].x, point.y - (int)tri.vertices[i].y};

        int crossProduct = edgeVector.x * pointVector.y - pointVector.x * edgeVector.y;
        if (crossProduct < 0)
            return false;
    }
    return true;
}

void drawFilledTriangleToPixelBuffer(Triangle tri, std::array<sf::Uint8, SCREEN_WIDTH * SCREEN_HEIGHT * 4>* pixelBuffer, int cr = 255, int cg = 255, int cb = 255)
{
    // Get area of triangle
    int x_min = std::min(std::min((int)tri.vertices[0].x, (int)tri.vertices[1].x), (int)tri.vertices[2].x);
    int x_max = std::max(std::max((int)tri.vertices[0].x, (int)tri.vertices[1].x), (int)tri.vertices[2].x);
    int y_min = std::min(std::min((int)tri.vertices[0].y, (int)tri.vertices[1].y), (int)tri.vertices[2].y);
    int y_max = std::max(std::max((int)tri.vertices[0].y, (int)tri.vertices[1].y), (int)tri.vertices[2].y);

    // Draw pixels in triangle
    for (int y = y_min; y < y_max; y++)
    {
        if (y < 0 || y > SCREEN_HEIGHT - 1)
            continue;
        for (int x = x_min; x < x_max; x++)
        {
            if (x < 0 || x > SCREEN_WIDTH - 1)
                continue;
            
            // Test if pixel is in triangle using cross product
            if (isPointInTriangle(tri, {x, y}))
            {
                // Draw pixel to buffer
                int pixel = x * 4 + y * SCREEN_WIDTH * 4;
                (*pixelBuffer)[pixel] = cr;
                (*pixelBuffer)[pixel + 1] = cg;
                (*pixelBuffer)[pixel + 2] = cb;
            }
        }
    }
}

int main()
{
    auto window = sf::RenderWindow{{SCREEN_WIDTH * SCREEN_RENDER_SCALE, SCREEN_HEIGHT * SCREEN_RENDER_SCALE}, "3d render test"};
    // window.setFramerateLimit(165);
    sf::Clock clock;

    std::vector<Triangle> cubeMesh = {
        {{-1, 1, 1}, {1, 1, 1}, {-1, -1, 1}},       {{1, 1, 1}, {1, -1, 1}, {-1, -1, 1}},       // front
        {{1, 1, -1}, {-1, 1, -1}, {1, -1, -1}},     {{-1, 1, -1}, {-1, -1, -1}, {1, -1, -1}},   // back
        {{1, 1, 1}, {1, 1, -1}, {1, -1, 1}},        {{1, 1, -1}, {1, -1, -1}, {1, -1, 1}},      // right
        {{-1, 1, -1}, {-1, 1, 1}, {-1, -1, -1}},    {{-1, 1, 1}, {-1, -1, 1}, {-1, -1, -1}},    // left
        {{-1, 1, -1}, {1, 1, -1}, {-1, 1, 1}},      {{1, 1, -1}, {1, 1, 1}, {-1, 1, 1}},        // top
        {{-1, -1, 1}, {1, -1, 1}, {-1, -1, -1}},    {{1, -1, 1}, {1, -1, -1}, {-1, -1, -1}}     // bottom
    };

    srand(time(NULL));
    std::vector<sf::Vector3f> cubePositions = {{0, 0, -10}};
    for (int i = 0; i < 10; i++)
    {
        // cubePositions.push_back(sf::Vector3f(rand() % 400, rand() % 400, rand() % 400));
    }

    sf::Vector3f rotation = {0, 0, 0};

    sf::Vector3f cameraPos = {0, 0, 0};
    sf::Vector3f cameraRot = {0, 0, 0};

    // Screen stuff
    std::array<sf::Uint8, SCREEN_WIDTH * SCREEN_HEIGHT * 4>* pixelBuffer = new std::array<sf::Uint8, SCREEN_WIDTH * SCREEN_HEIGHT * 4>;
    clearPixelBuffer(pixelBuffer);

    sf::Image renderImage;

    while (window.isOpen())
    {

        float dt = clock.restart().asSeconds();

        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        cameraPos.x += sin(cameraRot.y) * ((float)sf::Keyboard::isKeyPressed(sf::Keyboard::W) - (float)sf::Keyboard::isKeyPressed(sf::Keyboard::S)) * dt * 10;
        cameraPos.z += cos(cameraRot.y) * ((float)sf::Keyboard::isKeyPressed(sf::Keyboard::W) - (float)sf::Keyboard::isKeyPressed(sf::Keyboard::S)) * dt * 10;

        cameraPos.x += cos(cameraRot.y) * ((float)sf::Keyboard::isKeyPressed(sf::Keyboard::D) - (float)sf::Keyboard::isKeyPressed(sf::Keyboard::A)) * dt * 10;
        cameraPos.z += -sin(cameraRot.y) * ((float)sf::Keyboard::isKeyPressed(sf::Keyboard::D) - (float)sf::Keyboard::isKeyPressed(sf::Keyboard::A)) * dt * 10;

        cameraPos.y += ((float)sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) - (float)sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) * dt * 20;


        cameraRot.x = std::min(3.14f / 2.3f, std::max(
            -3.14f / 2.3f, cameraRot.x + ((float)sf::Keyboard::isKeyPressed(sf::Keyboard::I) - (float)sf::Keyboard::isKeyPressed(sf::Keyboard::K)) * dt * 2
        ));
        cameraRot.y += ((float)sf::Keyboard::isKeyPressed(sf::Keyboard::L) - (float)sf::Keyboard::isKeyPressed(sf::Keyboard::J)) * dt * 2;

        // rotation.x += dt / 8.0;
        // rotation.y += dt / 3.0;

        window.clear();

        clearPixelBuffer(pixelBuffer);

        for (sf::Vector3f cubePos : cubePositions)
        {
            // Draw cube triangles
            for (int i = 0; i < cubeMesh.size(); i++)
            {
                Triangle transformedTriangle;
                Triangle clipSpaceTriangle;
                Triangle projectedTriangle;

                // Transform vertices
                for (int j = 0; j < 3; j++)
                {
                    Vertex transformedVertex = cubeMesh[i].vertices[j];

                    // Scale
                    transformedVertex = transformedVertex;
                    transformedVertex.z = transformedVertex.z * 6;

                    // Rotate
                    transformedVertex = rotateVertex(transformedVertex, rotation - cubePos);

                    // Translate
                    transformedVertex.x -= cameraPos.x + cubePos.x;
                    transformedVertex.y -= cameraPos.y + cubePos.y;
                    transformedVertex.z -= cameraPos.z + cubePos.z;

                    // Store unprojected triangle
                    transformedTriangle.vertices[j] = transformedVertex;

                    // Rotate in camera view
                    transformedVertex = rotateVertexY(transformedVertex, -cameraRot.y);
                    transformedVertex = rotateVertexX(transformedVertex, -cameraRot.x);
                    
                    // To clip space
                    transformedVertex = vertexToClipSpace(transformedVertex, {SCREEN_WIDTH, SCREEN_HEIGHT}, 3.14 / 2.0, 0.01, 100);

                    clipSpaceTriangle.vertices[j] = transformedVertex;

                    // Perspective divide
                    transformedVertex.x /= transformedVertex.w;
                    transformedVertex.y /= transformedVertex.w;
                    transformedVertex.z /= transformedVertex.w;

                    // To screen space
                    transformedVertex.x = transformedVertex.x * SCREEN_WIDTH / 2.0 + SCREEN_WIDTH / 2.0;
                    transformedVertex.y = transformedVertex.y * SCREEN_HEIGHT / 2.0 + SCREEN_HEIGHT / 2.0;

                    projectedTriangle.vertices[j] = transformedVertex;
                }

                // Backface culling
                sf::Vector3f faceNormal = calculateTriNormal(projectedTriangle);
                if (faceNormal.z > 0)
                    continue;
                
                // Face lighting
                faceNormal = calculateTriNormal(transformedTriangle);

                sf::Vector3f lightVector = {0, 0, -1};

                float normalLength = sqrt(faceNormal.x * faceNormal.x + faceNormal.y * faceNormal.y + faceNormal.z * faceNormal.z);
                faceNormal = sf::Vector3f(faceNormal.x / normalLength, faceNormal.y / normalLength, faceNormal.z / normalLength);
                float dotProduct = lightVector.x * faceNormal.x + lightVector.y * faceNormal.y + lightVector.z * faceNormal.z;

                float lightColour = 70 + (255 - 70) * (dotProduct + 1) * 0.5;

                // Clip triangle
                std::vector<Triangle> triangles;
                triangles = clipTriangleToView(clipSpaceTriangle, makePlane({1, 0, 0}, {-1, 0, 0}));

                // Draw clipped triangles
                for (int i = 0; i < triangles.size(); i++)
                {
                    Triangle clippedProjectedTriangle = triangles[i];

                    for (int vertex = 0; vertex < 3; vertex++)
                    {
                        // Perspective divide
                        clippedProjectedTriangle.vertices[vertex].x /= clippedProjectedTriangle.vertices[vertex].w;
                        clippedProjectedTriangle.vertices[vertex].y /= clippedProjectedTriangle.vertices[vertex].w;
                        clippedProjectedTriangle.vertices[vertex].z /= clippedProjectedTriangle.vertices[vertex].w;

                        // To screen space
                        clippedProjectedTriangle.vertices[vertex].x = clippedProjectedTriangle.vertices[vertex].x * SCREEN_WIDTH / 2.0 + SCREEN_WIDTH / 2.0;
                        clippedProjectedTriangle.vertices[vertex].y = clippedProjectedTriangle.vertices[vertex].y * SCREEN_HEIGHT / 2.0 + SCREEN_HEIGHT / 2.0;
                    }

                    // Backface culling
                    sf::Vector3f faceNormal = calculateTriNormal(clippedProjectedTriangle);
                    if (faceNormal.z > 0)
                        continue;

                    // Draw triangle
                    // drawFilledTriangleToPixelBuffer(clippedProjectedTriangle, pixelBuffer, lightColour, lightColour, lightColour);
                    drawTriangleToPixelBuffer(clippedProjectedTriangle, pixelBuffer, 100, 100, 100);
                }


            }
        }

        renderImage.create(SCREEN_WIDTH, SCREEN_HEIGHT, pixelBuffer->data());

        sf::Texture texture;
        texture.loadFromImage(renderImage);
        sf::Sprite sprite;
        sprite.setTexture(texture, true);
        sprite.setScale(sf::Vector2f(SCREEN_RENDER_SCALE, SCREEN_RENDER_SCALE));

        window.draw(sprite);

        window.display();

        window.setTitle("3d render test - " + std::to_string(1.0 / dt) + "fps");

    }
}

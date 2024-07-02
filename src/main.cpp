#include <SFML/Graphics.hpp>
#include <array>
#include <math.h>
#include <iostream>

#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 150
#define SCREEN_RENDER_SCALE 5

struct Vertex {
    float x, y, z;
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
};

Vertex projectVertexToScreen(Vertex vertex, sf::Vector2u screenSize, float fov, float near, float far)
{
    Vertex projection;
    projection.x = vertex.x / (((float)screenSize.x / (float)screenSize.y) * vertex.z * tan(fov / 2.0));
    projection.x = projection.x * screenSize.x / 2.0 + screenSize.x / 2.0;
    projection.y = vertex.y / (vertex.z * tan(fov / 2.0));
    projection.y = projection.y * screenSize.y / 2.0 + screenSize.y / 2.0;
    projection.z = (-far - near) / (near - far) + (2 * far * near) / (vertex.z * (near - far));
    return projection;
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
    Vertex rotatedVertex;
    rotatedVertex.x = vertex.x;
    rotatedVertex.y = vertex.y * cos(angle) - vertex.z * sin(angle);
    rotatedVertex.z = vertex.y * sin(angle) + vertex.z * cos(angle);
    return rotatedVertex;
}

Vertex rotateVertexY(Vertex vertex, float angle)
{
    Vertex rotatedVertex;
    rotatedVertex.x = vertex.x * cos(angle) + vertex.z * sin(angle);
    rotatedVertex.y = vertex.y;
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

void clearPixelBuffer(sf::Uint8 (&pixelBuffer)[SCREEN_WIDTH * SCREEN_HEIGHT * 4])
{
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT * 4; i += 4)
    {
        pixelBuffer[i] = 0;
        pixelBuffer[i + 1] = 0;
        pixelBuffer[i + 2] = 0;
        pixelBuffer[i + 3] = 255;
    }
}

void drawLineToPixelBuffer(sf::Vector2u a, sf::Vector2u b, sf::Uint8 (&pixelBuffer)[SCREEN_WIDTH * SCREEN_HEIGHT * 4], int cr = 255, int cg = 255, int cb = 255)
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
            pixelBuffer[pixel] = cr;
            pixelBuffer[pixel + 1] = cg;
            pixelBuffer[pixel + 2] = cb;
        }
        x += xInc;
        y += yInc;
    }
}

void drawTriangleToPixelBuffer(Triangle tri, sf::Uint8 (&pixelBuffer)[SCREEN_WIDTH * SCREEN_HEIGHT * 4], int cr = 255, int cg = 255, int cb = 255)
{
    // Draw lines of triangle
    for (int i = 0; i < 3; i++)
    {
        Vertex vertexOne = tri.vertices[i];
        Vertex vertexTwo = tri.vertices[(i + 1) % 3];

        if (vertexOne.z < -1 || vertexOne.z > 1 || vertexTwo.z < -1 || vertexTwo.z > 1)
            continue;
        
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

void drawFilledTriangleToPixelBuffer(Triangle tri, sf::Uint8 (&pixelBuffer)[SCREEN_WIDTH * SCREEN_HEIGHT * 4], int cr = 255, int cg = 255, int cb = 255)
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
                int pixel = (int)(round(x) * 4) + (int)(round(y) * SCREEN_WIDTH * 4);
                pixelBuffer[pixel] = cr;
                pixelBuffer[pixel + 1] = cg;
                pixelBuffer[pixel + 2] = cb;
            }
        }
    }
}

int main()
{
    auto window = sf::RenderWindow{{SCREEN_WIDTH * SCREEN_RENDER_SCALE, SCREEN_HEIGHT * SCREEN_RENDER_SCALE}, "3d render test"};
    window.setFramerateLimit(165);
    sf::Clock clock;

    std::array<Vertex, 8> cube = {{
        {-1, 1, 1},       {1, 1, 1},
        {-1, -1, 1},      {1, -1, 1},
        {-1, 1, -1},      {1, 1, -1},
        {-1, -1, -1},     {1, -1, -1}
    }};

    std::array<std::array<int, 3>, 12> cubeTrisIndex = {{
        {0, 1, 2}, {1, 3, 2}, // front
        {5, 4, 7}, {4, 6, 7}, // back
        {1, 5, 3}, {5, 7, 3}, // right
        {4, 0, 6}, {0, 2, 6}, // left
        {4, 5, 0}, {5, 1, 0}, // top
        {2, 3, 6}, {3, 7, 6}  // bottom
    }};

    srand(time(NULL));
    std::vector<sf::Vector3f> cubePositions = {{0, 0, -100}};
    for (int i = 0; i < 10; i++)
    {
        // cubePositions.push_back(sf::Vector3f(rand() % 400, rand() % 400, rand() % 400));
    }

    sf::Vector3f rotation = {0, 0, 0};

    sf::Vector3f cameraPos = {0, 0, 0};
    sf::Vector3f cameraRot = {0, 0, 0};

    // Screen stuff
    sf::Uint8 pixelBuffer[SCREEN_WIDTH * SCREEN_HEIGHT * 4];
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

        cameraPos.x += sin(cameraRot.y) * ((float)sf::Keyboard::isKeyPressed(sf::Keyboard::W) - (float)sf::Keyboard::isKeyPressed(sf::Keyboard::S)) * dt * 20;
        cameraPos.z += cos(cameraRot.y) * ((float)sf::Keyboard::isKeyPressed(sf::Keyboard::W) - (float)sf::Keyboard::isKeyPressed(sf::Keyboard::S)) * dt * 20;

        cameraPos.x += cos(cameraRot.y) * ((float)sf::Keyboard::isKeyPressed(sf::Keyboard::D) - (float)sf::Keyboard::isKeyPressed(sf::Keyboard::A)) * dt * 20;
        cameraPos.z += -sin(cameraRot.y) * ((float)sf::Keyboard::isKeyPressed(sf::Keyboard::D) - (float)sf::Keyboard::isKeyPressed(sf::Keyboard::A)) * dt * 20;

        cameraPos.y += ((float)sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) - (float)sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) * dt * 20;


        cameraRot.x = std::min(3.14f / 2.3f, std::max(
            -3.14f / 2.3f, cameraRot.x + ((float)sf::Keyboard::isKeyPressed(sf::Keyboard::I) - (float)sf::Keyboard::isKeyPressed(sf::Keyboard::K)) * dt
        ));
        cameraRot.y += ((float)sf::Keyboard::isKeyPressed(sf::Keyboard::L) - (float)sf::Keyboard::isKeyPressed(sf::Keyboard::J)) * dt;

        rotation.x += dt / 8.0;
        rotation.y += dt / 3.0;

        window.clear();

        clearPixelBuffer(pixelBuffer);

        for (sf::Vector3f cubePos : cubePositions)
        {
            // Draw cube triangles
            for (int i = 0; i < cubeTrisIndex.size(); i++)
            {
                Triangle transformedTriangle;

                // Transform vertices
                for (int j = 0; j < 3; j++)
                {
                    Vertex transformedVertex = cube[cubeTrisIndex[i][j]];

                    // Rotate
                    transformedVertex = rotateVertex(transformedVertex, rotation - cubePos);

                    // Scale
                    transformedVertex = transformedVertex * 30;

                    // Translate
                    transformedVertex.x -= cameraPos.x + cubePos.x;
                    transformedVertex.y -= cameraPos.y + cubePos.y;
                    transformedVertex.z -= cameraPos.z + cubePos.z;

                    // Rotate in camera view
                    transformedVertex = rotateVertexY(transformedVertex, -cameraRot.y);
                    transformedVertex = rotateVertexX(transformedVertex, -cameraRot.x);

                    // Project
                    transformedVertex = projectVertexToScreen(transformedVertex, {SCREEN_WIDTH, SCREEN_HEIGHT}, 3.14 / 2.0, 0.001, 1000);

                    transformedTriangle.vertices[j] = transformedVertex;
                }

                // Backface culling
                sf::Vector3f faceNormal = calculateTriNormal(transformedTriangle);
                if (faceNormal.z > 0)
                    continue;

                // Draw triangle
                drawFilledTriangleToPixelBuffer(transformedTriangle, pixelBuffer);
                drawTriangleToPixelBuffer(transformedTriangle, pixelBuffer, 100, 100, 100);

            }
        }

        renderImage.create(SCREEN_WIDTH, SCREEN_HEIGHT, pixelBuffer);

        sf::Texture texture;
        texture.loadFromImage(renderImage);
        sf::Sprite sprite;
        sprite.setTexture(texture, true);
        sprite.setScale(sf::Vector2f(SCREEN_RENDER_SCALE, SCREEN_RENDER_SCALE));

        window.draw(sprite);

        window.display();

    }
}

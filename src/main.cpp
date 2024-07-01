#include <SFML/Graphics.hpp>
#include <array>
#include <math.h>
#include <iostream>

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

int main()
{
    auto window = sf::RenderWindow{{1280, 720}, "3d render test"};
    window.setFramerateLimit(165);
    sf::Clock clock;

    std::array<Vertex, 8> cube = {{
        {-1, 1, 1},       {1, 1, 1},
        {-1, -1, 1},      {1, -1, 1},
        {-1, 1, -1},      {1, 1, -1},
        {-1, -1, -1},     {1, -1, -1}
    }};

    std::array<int, 12 * 2> cubeDrawIndex = {0, 1, 1, 3, 0, 2, 2, 3, 4, 5, 5, 7, 4, 6, 6, 7, 0, 4, 1, 5, 2, 6, 3, 7};

    srand(time(NULL));
    std::vector<sf::Vector3f> cubePositions = {{0, 0, 0}};
    for (int i = 0; i < 7; i++)
    {
        cubePositions.push_back(sf::Vector3f(rand() % 200, rand() % 200, rand() % 200));
    }

    sf::Vector3f rotation = {0, 0, 0};

    sf::Vector3f cameraPos = {0, 0, 0};
    sf::Vector3f cameraRot = {0, 0, 0};

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

        rotation.x += dt;
        rotation.y += dt;
        rotation.z += dt;

        window.clear();

        for (sf::Vector3f cubePos : cubePositions)
        {
            // Draw cube
            for (int i = 0; i < cubeDrawIndex.size(); i += 2)
            {
                Vertex vertexOne = cube[cubeDrawIndex[i]];
                Vertex vertexTwo = cube[cubeDrawIndex[i + 1]];

                // Rotate cube
                // vertexOne = rotateVertex(vertexOne, rotation - cubePos);
                // vertexTwo = rotateVertex(vertexTwo, rotation - cubePos);

                // Scale cube
                vertexOne = vertexOne * 30;
                vertexTwo = vertexTwo * 30;

                // Move cube
                vertexOne.x -= cameraPos.x + cubePos.x;
                vertexOne.y -= cameraPos.y + cubePos.y;
                vertexOne.z -= cameraPos.z + cubePos.z;
                vertexTwo.x -= cameraPos.x + cubePos.x;
                vertexTwo.y -= cameraPos.y + cubePos.y;
                vertexTwo.z -= cameraPos.z + cubePos.z;

                // Rotate in camera view
                vertexOne = rotateVertexY(vertexOne, -cameraRot.y);
                vertexOne = rotateVertexX(vertexOne, -cameraRot.x);
                vertexTwo = rotateVertexY(vertexTwo, -cameraRot.y);
                vertexTwo = rotateVertexX(vertexTwo, -cameraRot.x);

                // Project
                vertexOne = projectVertexToScreen(vertexOne, {1280, 720}, 3.14 / 2.0, 0.001, 1000);
                vertexTwo = projectVertexToScreen(vertexTwo, {1280, 720}, 3.14 / 2.0, 0.001, 1000);

                if (vertexOne.z < -1 || vertexOne.z > 1 || vertexTwo.z < -1 || vertexTwo.z > 1)
                    continue;

                sf::VertexArray line(sf::LinesStrip, 2);
                line[0].position = sf::Vector2f(vertexOne.x, vertexOne.y);
                line[1].position = sf::Vector2f(vertexTwo.x, vertexTwo.y);

                window.draw(line);
            }
        }

        window.display();

    }
}

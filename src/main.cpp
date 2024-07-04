#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <memory>
#include <math.h>
#include <iostream>

#include "BrnRenderer.hpp"
#include "Mathematics/Plane.hpp"
#include "Renderer/Clipping.hpp"
#include "Renderer/Projection.hpp"
#include "Renderer/Rotation.hpp"

int main()
{
    srand(time(NULL));

    brn::BrnRenderer renderer;

    sf::Clock clock;

    brn::Mesh cubeMesh = brn::createCubeMesh();

    std::vector<std::array<brn::Vector3, 3>> cubes;

    brn::Vector3 cameraPos = {0, 0, 0};
    brn::Vector3 cameraRot = {0, 0, 0};

    float totalTime = 0;

    while (renderer.windowOpen())
    {

        float dt = clock.restart().asSeconds();

        totalTime += dt;
        if (totalTime >= 0.01)
        {
            totalTime = 0;
            cubes.push_back({{{0, -10, -30}, {(float)(rand() % 25) - 12, (float)(rand() % 20) + 25, (float)(rand() % 25) - 12}, {(float)(rand() % 7), (float)(rand() % 7), (float)(rand() % 7)}}});
        }

        for (auto event = sf::Event{}; renderer.getWindow().pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                renderer.closeWindow();
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

        // Update camera
        renderer.setCamera(cameraPos, cameraRot);

        renderer.clearScreen();

        for (int i = 0; i < cubes.size(); i++)
        {
            std::array<brn::Vector3, 3>& cube = cubes[i];

            // Apply physics
            cube[0].x += cube[1].x * dt;
            cube[0].y += cube[1].y * dt;
            cube[0].z += cube[1].z * dt;
            cube[1].y -= 100 * dt;

            if (cube[0].y < -50)
            {
                cubes.erase(cubes.begin() + i);
                i--;
                continue;
            }

            // Draw cube
            renderer.drawMesh(cubeMesh, cube[0], cube[2], {1, 1, 1});

        }

        renderer.updateScreen();

        renderer.setWindowTitle("BrnRenderer - " + std::to_string((int)(1.0 / dt)) + "FPS");

    }
}

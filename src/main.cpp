#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <memory>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include "BrnRenderer.hpp"
#include "Mathematics/Plane.hpp"
#include "Renderer/Clipping.hpp"
#include "Renderer/Projection.hpp"
#include "Renderer/Rotation.hpp"

int main()
{
    srand(time(NULL));

    brn::BrnRenderer renderer;
    renderer.setLightDirection({0, -0.75, -1});

    sf::Clock clock;

    brn::Mesh cubeMesh = brn::createCubeMesh(1, 1, 1, 200, 200, 200);
    brn::Mesh cubeMeshRed = brn::createCubeMesh(1, 1, 1, 220, 60, 60);
    brn::Mesh pyramidMesh = brn::createPyramidSqMesh(1, 1, 1, 255, 255, 50);

    sf::Image* testTexture = new sf::Image;
    testTexture->loadFromFile("brick.png");

    std::vector<brn::Vector3> pyramidPos;
    for (int i = 0; i < 20; i++)
    {
        pyramidPos.push_back({(float)(rand() % 80 - 40), 3, (float)(rand() % 80 - 80)});
    }

    brn::Vector3 cameraPos = {0, 0, 0};
    brn::Vector3 cameraRot = {0, 0, 0};

    float time = 0;

    while (renderer.windowOpen())
    {

        float dt = clock.restart().asSeconds();
        time += dt;

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

        renderer.clearScreen(40, 40, 40);

        for (brn::Vector3 pos : pyramidPos)
        {
            renderer.drawMesh(pyramidMesh, pos, {M_PI, time, 0}, {1, 1, 1});
        }
        renderer.drawMesh(cubeMesh, {0, 0, -10}, {0, time, time / 3.0f}, {3, 3, 3});
        renderer.drawMesh(cubeMeshRed, {-2, 2, -10}, {0, time * 1.2f, time / 2.0f}, {3, 3, 3}, testTexture);

        renderer.updateScreen();

        renderer.setWindowTitle("BrnRenderer - " + std::to_string((int)(1.0 / dt)) + "FPS");

    }

    // delete testTexture;
    // testTexture = nullptr;

}

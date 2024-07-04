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
    renderer.setLightDirection({0, -0.75, -1});

    sf::Clock clock;

    brn::Mesh cubeMeshWhite = brn::createCubeMesh(1, 1, 1, 255, 255, 255);

    brn::Vector3 cameraPos = {0, 0, 0};
    brn::Vector3 cameraRot = {0, 0, 0};

    while (renderer.windowOpen())
    {

        float dt = clock.restart().asSeconds();

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

        renderer.drawMesh(cubeMeshWhite, {0, 0, -10}, {0, 0, 0}, {4, 4, 4});

        renderer.updateScreen();

        renderer.setWindowTitle("BrnRenderer - " + std::to_string((int)(1.0 / dt)) + "FPS");

    }
}

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

    brn::BrnRenderer renderer(1280, 720, 1);
    renderer.setLightDirection({0, -0.75, -1});

    sf::Clock clock;

    brn::Mesh cubeMesh = brn::createCubeMesh();
    brn::Mesh pyramidMesh = brn::createPyramidSqMesh();
    brn::Mesh groundMesh = brn::createPlaneMesh(4, 4);
    brn::Mesh wallMesh = brn::createPlaneMesh(3, 8);

    sf::Image* dirtTexture = new sf::Image;
    dirtTexture->loadFromFile("dirt.png");
    sf::Image* brickTexture = new sf::Image;
    brickTexture->loadFromFile("brick.png");
    sf::Image* diamondTexture = new sf::Image;
    diamondTexture->loadFromFile("diamond.png");

    std::vector<brn::Vector3> pyramidPos;
    for (int i = 0; i < 100; i++)
    {
        pyramidPos.push_back({(float)(rand() % 20 - 10), 3, (float)(rand() % 20 - 10)});
    }

    brn::Vector3 cameraPos = {0, -5, 0};
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

        // cameraPos.y += ((float)sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) - (float)sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) * dt * 20;


        cameraRot.x = std::min(3.14f / 2.3f, std::max(
            -3.14f / 2.3f, cameraRot.x + ((float)sf::Keyboard::isKeyPressed(sf::Keyboard::I) - (float)sf::Keyboard::isKeyPressed(sf::Keyboard::K)) * dt * 2
        ));
        cameraRot.y += ((float)sf::Keyboard::isKeyPressed(sf::Keyboard::L) - (float)sf::Keyboard::isKeyPressed(sf::Keyboard::J)) * dt * 2;

        // Update camera
        renderer.setCamera(cameraPos, cameraRot);

        renderer.clearScreen(40, 40, 40);

        renderer.drawMesh(groundMesh, {0, 0, 0}, {M_PI / 2, 0, 0}, {10, 10, 1}, dirtTexture);
        renderer.drawMesh(wallMesh, {0, 7.5, 20}, {0, 0, 0}, {5, 5, 1}, brickTexture);
        renderer.drawMesh(wallMesh, {0, 7.5, -20}, {0, M_PI, 0}, {5, 5, 1}, brickTexture);
        renderer.drawMesh(wallMesh, {20, 7.5, 0}, {0, M_PI / 2, 0}, {5, 5, 1}, brickTexture);
        renderer.drawMesh(wallMesh, {-20, 7.5, 0}, {0, -M_PI / 2, 0}, {5, 5, 1}, brickTexture);

        renderer.updateScreen();

        renderer.setWindowTitle("BrnRenderer - " + std::to_string((int)(1.0 / dt)) + "FPS");

    }

    delete dirtTexture;
    delete brickTexture;
    delete diamondTexture;

}

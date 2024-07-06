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

    brn::BrnRenderer renderer(320, 240, 3);
    renderer.setLightDirection({0, -0.75, -1});

    sf::Clock clock;

    // brn::Mesh cubeMesh = brn::createCubeMesh();
    // brn::Mesh pyramidMesh = brn::createPyramidSqMesh();
    // brn::Mesh groundMesh = brn::createPlaneMesh(4, 4);
    // brn::Mesh wallMesh = brn::createPlaneMesh(3, 8);

    // brn::Mesh coolMesh = brn::loadMeshFromFile("untitled.obj");
    // brn::Mesh astronaut = brn::loadMeshFromFile("Astronaut.obj");
    // brn::Mesh shuttle = brn::loadMeshFromFile("shuttle.obj");
    // brn::Mesh andy = brn::loadMeshFromFile("andy.obj");
    // brn::Mesh gunMesh = brn::loadMeshFromFile("gun.obj");

    // sf::Image* dirtTexture = new sf::Image;
    // dirtTexture->loadFromFile("dirt.png");
    // sf::Image* brickTexture = new sf::Image;
    // brickTexture->loadFromFile("brick.png");
    // sf::Image* diamondTexture = new sf::Image;
    // diamondTexture->loadFromFile("diamond.png");

    // sf::Image* andyTexture = new sf::Image;
    // andyTexture->loadFromFile("Andy_Diffuse.png");

    // sf::Image* astronautTex = new sf::Image;
    // astronautTex->loadFromFile("Astronaut_BaseColor.png");

    // sf::Image* shuttleTex = new sf::Image;
    // shuttleTex->loadFromFile("SpaceShuttle_BaseColor.png");

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
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::P)
                    renderer.toggleWireframeRender();
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

        // renderer.drawMesh(astronaut, {0, -10, -10}, {M_PI, time, 0}, {2, 2, 2}, astronautTex);
        // renderer.drawMesh(shuttle, {0, -10, -20}, {M_PI, time, 0}, {0.25, 0.25, 0.25}, shuttleTex);
        // renderer.drawMesh(andy, {0, -10, -10}, {M_PI, time, 0}, {2, 2, 2}, andyTexture);
        // renderer.drawMesh(gunMesh, {0, 0, -10}, {0, 0, 0}, {15, 15, 15});

        renderer.updateScreen();

        renderer.setWindowTitle("BrnRenderer - " + std::to_string((int)(1.0 / dt)) + "FPS");

    }

}

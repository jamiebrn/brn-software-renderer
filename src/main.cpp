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
    // auto window = sf::RenderWindow{{SCREEN_WIDTH * SCREEN_RENDER_SCALE, SCREEN_HEIGHT * SCREEN_RENDER_SCALE}, "3d render test"};
    // window.setFramerateLimit(165);
    brn::BrnRenderer renderer("3d renderer test");

    sf::Clock clock;

    std::vector<brn::Triangle> cubeMesh = {
        {{-1, 1, 1}, {1, 1, 1}, {-1, -1, 1}},       {{1, 1, 1}, {1, -1, 1}, {-1, -1, 1}},       // front
        {{1, 1, -1}, {-1, 1, -1}, {1, -1, -1}},     {{-1, 1, -1}, {-1, -1, -1}, {1, -1, -1}},   // back
        {{1, 1, 1}, {1, 1, -1}, {1, -1, 1}},        {{1, 1, -1}, {1, -1, -1}, {1, -1, 1}},      // right
        {{-1, 1, -1}, {-1, 1, 1}, {-1, -1, -1}},    {{-1, 1, 1}, {-1, -1, 1}, {-1, -1, -1}},    // left
        {{-1, 1, -1}, {1, 1, -1}, {-1, 1, 1}},      {{1, 1, -1}, {1, 1, 1}, {-1, 1, 1}},        // top
        {{-1, -1, 1}, {1, -1, 1}, {-1, -1, -1}},    {{1, -1, 1}, {1, -1, -1}, {-1, -1, -1}}     // bottom
    };

    srand(time(NULL));
    std::vector<brn::Vector3> cubePositions = {{0, 0, -10}};
    for (int i = 0; i < 10; i++)
    {
        cubePositions.push_back(brn::Vector3{(float)(rand() % 40), (float)(rand() % 40), (float)(rand() % 40)});
    }

    brn::Vector3 rotation = {0, 0, 0};

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

        rotation.x += dt / 8.0;
        rotation.y += dt / 3.0;

        renderer.clearScreen();

        for (brn::Vector3 cubePos : cubePositions)
        {
            // Draw cube triangles
            for (int i = 0; i < cubeMesh.size(); i++)
            {
                brn::Triangle transformedTriangle;
                brn::Triangle clipSpaceTriangle;

                // Transform vertices to (homogeneous) clip space
                for (int j = 0; j < 3; j++)
                {
                    brn::Vertex transformedVertex = cubeMesh[i].vertices[j];

                    // Scale
                    transformedVertex = transformedVertex;
                    transformedVertex.z = transformedVertex.z * 6;

                    // Rotate
                    transformedVertex = rotateVertex(transformedVertex, rotation);

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
                    transformedVertex = brn::vertexToClipSpace(transformedVertex, SCREEN_WIDTH, SCREEN_HEIGHT, 3.14 / 2.0, 0.1, 100);

                    clipSpaceTriangle.vertices[j] = transformedVertex;
                }
                
                // Face lighting
                brn::Vector3 faceNormal = transformedTriangle.calculateNormal();

                brn::Vector3 lightVector = {0, 0, -1};

                float normalLength = sqrt(faceNormal.x * faceNormal.x + faceNormal.y * faceNormal.y + faceNormal.z * faceNormal.z);
                faceNormal = {faceNormal.x / normalLength, faceNormal.y / normalLength, faceNormal.z / normalLength};
                float dotProduct = lightVector.x * faceNormal.x + lightVector.y * faceNormal.y + lightVector.z * faceNormal.z;

                float lightColour = 70 + (255 - 70) * (dotProduct + 1) * 0.5;

                // Clip triangle
                std::vector<brn::Triangle> triangles = {clipSpaceTriangle};
                brn::clipTriangles(triangles, brn::Plane({-1, 0, 0}, {1, 0, 0}));
                brn::clipTriangles(triangles, brn::Plane({0, 1, 0}, {0, -1, 0}));
                brn::clipTriangles(triangles, brn::Plane({1, 0, 0}, {-1, 0, 0}));
                brn::clipTriangles(triangles, brn::Plane({0, -1, 0}, {0, 1, 0}));
                brn::clipTriangles(triangles, brn::Plane({0, 0, -1}, {0, 0, 1}));
                brn::clipTriangles(triangles, brn::Plane({0, 0, 1}, {0, 0, -1}));

                // Draw clipped triangles
                for (int i = 0; i < triangles.size(); i++)
                {
                    brn::Triangle clippedProjectedTriangle = triangles[i];

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
                    brn::Vector3 faceNormal = clippedProjectedTriangle.calculateNormal();
                    if (faceNormal.z > 0)
                        continue;

                    // Draw triangle
                    renderer.drawFilledTriangleToPixelBuffer(clippedProjectedTriangle, lightColour, lightColour, lightColour);
                    // drawTriangleToPixelBuffer(clippedProjectedTriangle, pixelBuffer, 100, 100, 100);
                }


            }
        }

        renderer.updateScreen();

        renderer.setWindowTitle("3d render test - " + std::to_string(1.0 / dt) + "fps");

    }
}

#pragma once

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 400
#define SCREEN_RENDER_SCALE 2

#include <SFML/Graphics.hpp>
#include <string>
#include <array>
#include <math.h>
#include <iostream>
#include <queue>

#include "Mathematics/Vertex.hpp"
#include "Mathematics/Triangle.hpp"
#include "Mathematics/Mesh.hpp"
#include "Mathematics/Plane.hpp"
#include "Renderer/Clipping.hpp"
#include "Renderer/Projection.hpp"
#include "Renderer/Rotation.hpp"

namespace brn
{

class BrnRenderer
{

public:
    // BrnRenderer(int screenWidth, int screenHeight, int renderWidth, int renderHeight, std::string title);
    BrnRenderer(std::string title = "BrnRenderer");

    void drawMesh(const Mesh& mesh, const Vector3& position, const Vector3& rotation, const Vector3& scale);

    void drawLineToPixelBuffer(const Vertex& a, const Vertex& b, int cr = 255, int cg = 255, int cb = 255);
    void drawTriangleToPixelBuffer(const Triangle& tri, int cr = 255, int cg = 255, int cb = 255);
    void drawFilledTriangleToPixelBuffer(const Triangle& tri, int cr = 255, int cg = 255, int cb = 255);

    void setCamera(const Vector3& position, const Vector3& rotation);
    
    void clearScreen();
    void updateScreen();

    bool windowOpen();

    void setWindowTitle(const std::string& title);

    sf::RenderWindow& getWindow();

    void closeWindow();

private:
    void clearPixelBuffer();
    void clearDepthBuffer();

private:
    std::array<sf::Uint8, SCREEN_WIDTH * SCREEN_HEIGHT * 4>* pixelBuffer;
    std::array<float, SCREEN_WIDTH * SCREEN_HEIGHT>* depthBuffer;

    static const std::array<brn::Plane, 6> clippingPlanes;

    Vector3 cameraPosition;
    Vector3 cameraRotation;

    sf::RenderWindow window;

};

}

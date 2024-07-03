#pragma once

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCREEN_RENDER_SCALE 1

#include <SFML/Graphics.hpp>
#include <string>
#include <array>
#include <math.h>
#include <iostream>

#include "Mathematics/Vertex.hpp"
#include "Mathematics/Triangle.hpp"

namespace brn
{

class BrnRenderer
{

public:
    // BrnRenderer(int screenWidth, int screenHeight, int renderWidth, int renderHeight, std::string title);
    BrnRenderer(std::string title);

    void drawLineToPixelBuffer(Vertex a, Vertex b, int cr = 255, int cg = 255, int cb = 255);
    void drawTriangleToPixelBuffer(Triangle tri, int cr = 255, int cg = 255, int cb = 255);
    void drawFilledTriangleToPixelBuffer(Triangle tri, int cr = 255, int cg = 255, int cb = 255);

    void clearScreen();
    void updateScreen();

    bool windowOpen();

    void setWindowTitle(std::string title);

    sf::RenderWindow& getWindow();

    void closeWindow();

private:
    void clearPixelBuffer();
    void clearDepthBuffer();

private:
    std::array<sf::Uint8, SCREEN_WIDTH * SCREEN_HEIGHT * 4>* pixelBuffer;
    std::array<float, SCREEN_WIDTH * SCREEN_HEIGHT>* depthBuffer;

    sf::RenderWindow window;

};

}

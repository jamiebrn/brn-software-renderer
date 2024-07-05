#pragma once

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCREEN_RENDER_SCALE 1

#include <SFML/Graphics.hpp>
#include <string>
#include <array>
#include <math.h>
#include <iostream>
#include <queue>
#include <cstdint>

#include "Mathematics/Vertex.hpp"
#include "Mathematics/Triangle.hpp"
#include "Mathematics/Mesh.hpp"
#include "Mathematics/Plane.hpp"
#include "Renderer/Clipping.hpp"
#include "Renderer/Projection.hpp"
#include "Renderer/Rotation.hpp"
#include "Renderer/Colour.hpp"

namespace brn
{

class BrnRenderer
{

public:
    // BrnRenderer(int screenWidth, int screenHeight, int renderWidth, int renderHeight, std::string title);
    BrnRenderer(std::string title = "BrnRenderer");

    void drawMesh(const Mesh& mesh, const Vector3& position, const Vector3& rotation, const Vector3& scale, sf::Image* texture = nullptr);

    void drawPixelToBuffer(const Vector2i& position, const Colour& colour, float depth = 0);
    void drawLineToPixelBuffer(const Vertex& a, const Vertex& b);
    void drawTriangleToPixelBuffer(const Triangle& tri);
    void drawFilledTriangleToPixelBuffer(const Triangle& tri, float lightStrength = 1.0f, sf::Image* texture = nullptr);

    void setCamera(const Vector3& position, const Vector3& rotation);
    void setLightDirection(const Vector3& direction);
    
    void clearScreen(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0);
    void updateScreen();

    bool windowOpen();

    void setWindowTitle(const std::string& title);

    sf::RenderWindow& getWindow();

    void closeWindow();

private:
    void clearPixelBuffer(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0);
    void clearDepthBuffer();

    float triangleEdgeCrossProduct(const Vector2& v1, const Vector2& v2, const Vector2& point);
    bool isTriangleTopOrLeftEdge(const Vector2& v1, const Vector2& v2);

    Colour sampleFromTexture(sf::Image* texture, float u, float v);

private:
    std::array<sf::Uint8, SCREEN_WIDTH * SCREEN_HEIGHT * 4>* pixelBuffer;
    std::array<float, SCREEN_WIDTH * SCREEN_HEIGHT>* depthBuffer;

    static const std::array<brn::Plane, 6> clippingPlanes;

    Vector3 cameraPosition;
    Vector3 cameraRotation;

    Vector3 lightDirection;

    sf::RenderWindow window;

};

}

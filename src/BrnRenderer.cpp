#include "BrnRenderer.hpp"

brn::BrnRenderer::BrnRenderer(std::string title)
{
    // Create window
    window.create({SCREEN_WIDTH * SCREEN_RENDER_SCALE, SCREEN_HEIGHT * SCREEN_RENDER_SCALE}, title);

    // Initialise pixel and depth buffers
    pixelBuffer = new std::array<sf::Uint8, SCREEN_WIDTH * SCREEN_HEIGHT * 4>;
    depthBuffer = new std::array<float, SCREEN_WIDTH * SCREEN_HEIGHT>;

    clearScreen();
}

void brn::BrnRenderer::clearPixelBuffer()
{
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT * 4; i += 4)
    {
        (*pixelBuffer)[i] = 0;
        (*pixelBuffer)[i + 1] = 0;
        (*pixelBuffer)[i + 2] = 0;
        (*pixelBuffer)[i + 3] = 255;
    }
}

void brn::BrnRenderer::clearDepthBuffer()
{
    (*depthBuffer).fill(INFINITY);
}

void brn::BrnRenderer::drawLineToPixelBuffer(Vertex a, Vertex b, int cr, int cg, int cb)
{
    int dx = (unsigned int)b.x - (unsigned int)a.x;
    int dy = (unsigned int)b.y - (unsigned int)a.y;
    int steps = std::max(std::abs(dx), std::abs(dy));
    float xInc = (float)dx / (float)steps;
    float yInc = (float)dy / (float)steps;
    float x = (unsigned int)a.x, y = (unsigned int)a.y;

    for (int i = 0; i <= steps; i++)
    {
        if (x > 0 && x < SCREEN_WIDTH - 1 && y > 0 && y < SCREEN_HEIGHT - 1)
        {
            // Draw pixel to buffer
            int pixel = (int)(round(x) * 4) + (int)(round(y) * SCREEN_WIDTH * 4);
            (*pixelBuffer)[pixel] = cr;
            (*pixelBuffer)[pixel + 1] = cg;
            (*pixelBuffer)[pixel + 2] = cb;
        }
        x += xInc;
        y += yInc;
    }
}

void brn::BrnRenderer::drawTriangleToPixelBuffer(Triangle tri, int cr, int cg, int cb)
{
    // Draw lines of triangle
    for (int i = 0; i < 3; i++)
    {
        Vertex vertexOne = tri.vertices[i];
        Vertex vertexTwo = tri.vertices[(i + 1) % 3];
        
        drawLineToPixelBuffer(vertexOne, vertexTwo, cr, cg, cb);
    }
}

void brn::BrnRenderer::drawFilledTriangleToPixelBuffer(Triangle tri, int cr, int cg, int cb)
{
    // Get area of triangle
    int x_min = std::min(std::min((int)tri.vertices[0].x, (int)tri.vertices[1].x), (int)tri.vertices[2].x);
    int x_max = std::max(std::max((int)tri.vertices[0].x, (int)tri.vertices[1].x), (int)tri.vertices[2].x);
    int y_min = std::min(std::min((int)tri.vertices[0].y, (int)tri.vertices[1].y), (int)tri.vertices[2].y);
    int y_max = std::max(std::max((int)tri.vertices[0].y, (int)tri.vertices[1].y), (int)tri.vertices[2].y);

    // Calculate average triangle depth
    float depth = (tri.vertices[0].w + tri.vertices[1].w + tri.vertices[2].w) / 3;

    // Draw pixels in triangle
    for (int y = y_min; y < y_max; y++)
    {
        if (y < 0 || y > SCREEN_HEIGHT - 1)
            continue;
        for (int x = x_min; x < x_max; x++)
        {
            if (x < 0 || x > SCREEN_WIDTH - 1)
                continue;
            
            if ((*depthBuffer)[x + y * SCREEN_WIDTH] < depth)
                continue;
            
            // Test if pixel is in triangle using cross product
            if (tri.isPointInTriangle({x, y}))
            {
                // Draw pixel to buffer
                int pixel = x * 4 + y * SCREEN_WIDTH * 4;
                (*pixelBuffer)[pixel] = cr;
                (*pixelBuffer)[pixel + 1] = cg;
                (*pixelBuffer)[pixel + 2] = cb;
                // Draw to depth buffer
                (*depthBuffer)[x + y * SCREEN_WIDTH] = depth;
            }
        }
    }
}

void brn::BrnRenderer::clearScreen()
{
    window.clear();
    clearPixelBuffer();
    clearDepthBuffer();
}

void brn::BrnRenderer::updateScreen()
{
    sf::Image renderImage;
    renderImage.create(SCREEN_WIDTH, SCREEN_HEIGHT, pixelBuffer->data());

    sf::Texture texture;
    texture.loadFromImage(renderImage);
    
    sf::Sprite sprite;
    sprite.setTexture(texture, true);
    sprite.setScale(sf::Vector2f(SCREEN_RENDER_SCALE, SCREEN_RENDER_SCALE));

    window.draw(sprite);

    window.display();
}

bool brn::BrnRenderer::windowOpen()
{
    return window.isOpen();
}

void brn::BrnRenderer::setWindowTitle(std::string title)
{
    window.setTitle(title);
}

sf::RenderWindow& brn::BrnRenderer::getWindow()
{
    return window;
}

void brn::BrnRenderer::closeWindow()
{
    window.close();

    // Free buffer memory
    delete pixelBuffer;
    pixelBuffer = nullptr;
    delete depthBuffer;
    depthBuffer = nullptr;
}

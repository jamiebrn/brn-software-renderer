#include "BrnRenderer.hpp"

const std::array<brn::Plane, 6> brn::BrnRenderer::clippingPlanes = {
    Plane({-1, 0, 0}, {1, 0, 0}),
    Plane({0, 1, 0}, {0, -1, 0}),
    Plane({1, 0, 0}, {-1, 0, 0}),
    Plane({0, -1, 0}, {0, 1, 0}),
    Plane({0, 0, -1}, {0, 0, 1}),
    Plane({0, 0, 1}, {0, 0, -1})
};

brn::BrnRenderer::BrnRenderer(unsigned int screenWidth, unsigned int screenHeight, unsigned int screenRenderScale, std::string title)
{
    // Create window
    window.create({screenWidth * screenRenderScale, screenHeight * screenRenderScale}, title);

    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->screenRenderScale = screenRenderScale;

    // Initialise pixel and depth buffers
    pixelBuffer = new sf::Uint8[screenWidth * screenHeight * 4];
    depthBuffer = new float[screenWidth * screenHeight];

    // Reset values
    cameraPosition = {0, 0, 0};
    cameraRotation = {0, 0, 0};
    lightDirection = {0, 0, -1};

    clearScreen();
}

void brn::BrnRenderer::clearPixelBuffer(uint8_t r, uint8_t g, uint8_t b)
{
    for (int i = 0; i < screenWidth * screenHeight * 4; i += 4)
    {
        pixelBuffer[i] = r;
        pixelBuffer[i + 1] = g;
        pixelBuffer[i + 2] = b;
        pixelBuffer[i + 3] = 255;
    }
}

void brn::BrnRenderer::clearDepthBuffer()
{
    for (int i = 0; i < screenWidth * screenHeight; i++)
    {
        depthBuffer[i] = INFINITY;
    }
}

void brn::BrnRenderer::drawMesh(const Mesh& mesh, const Vector3& position, const Vector3& rotation, const Vector3& scale, sf::Image* texture)
{

    for (int i = 0; i < mesh.triangles.size(); i++)
    {
        brn::Triangle transformedTriangle;
        brn::Triangle clipSpaceTriangle;

        // Transform vertices to (homogeneous) clip space
        for (int j = 0; j < 3; j++)
        {
            brn::Vertex transformedVertex = mesh.triangles[i].vertices[j];

            // Scale
            transformedVertex.x *= scale.x;
            transformedVertex.y *= scale.y;
            transformedVertex.z *= scale.z;

            // Rotate
            transformedVertex = rotateVertex(transformedVertex, rotation);

            // Translate
            transformedVertex.x -= cameraPosition.x + position.x;
            transformedVertex.y -= cameraPosition.y + position.y;
            transformedVertex.z -= cameraPosition.z + position.z;

            // Store unprojected triangle
            transformedTriangle.vertices[j] = transformedVertex;

            // Rotate in camera view
            transformedVertex = rotateVertexY(transformedVertex, -cameraRotation.y);
            transformedVertex = rotateVertexX(transformedVertex, -cameraRotation.x);
            
            // To clip space
            brn::vertexToClipSpace(transformedVertex, screenWidth, screenHeight, 3.14 / 2.0, 0.1, 100);

            clipSpaceTriangle.vertices[j] = transformedVertex;
        }
        
        // Face lighting
        brn::Vector3 faceNormal = transformedTriangle.calculateNormal();

        float normalLength = sqrt(faceNormal.x * faceNormal.x + faceNormal.y * faceNormal.y + faceNormal.z * faceNormal.z);
        faceNormal = {faceNormal.x / normalLength, faceNormal.y / normalLength, faceNormal.z / normalLength};
        float dotProduct = lightDirection.x * faceNormal.x + lightDirection.y * faceNormal.y + lightDirection.z * faceNormal.z;

        // Light strength between 1 and 0.35
        float lightStrength = (dotProduct + 1) * (0.65 / 2) + 0.35;

        // Clip triangle
        std::queue<brn::Triangle> triangles;
        triangles.push(clipSpaceTriangle);
        for (int i = 0; i < clippingPlanes.size(); i++)
        {
            brn::clipTriangles(triangles, clippingPlanes[i]);
        }

        // Draw clipped triangles
        while (triangles.size() > 0)
        {
            brn::Triangle& clippedProjectedTriangle = triangles.front();

            for (int vertex = 0; vertex < 3; vertex++)
            {
                // Perspective divide
                clippedProjectedTriangle.vertices[vertex].x /= clippedProjectedTriangle.vertices[vertex].w;
                clippedProjectedTriangle.vertices[vertex].y /= clippedProjectedTriangle.vertices[vertex].w;
                clippedProjectedTriangle.vertices[vertex].z /= clippedProjectedTriangle.vertices[vertex].w;

                // Perspective correct texture coordinates
                clippedProjectedTriangle.vertices[vertex].u /= clippedProjectedTriangle.vertices[vertex].w;
                clippedProjectedTriangle.vertices[vertex].v /= clippedProjectedTriangle.vertices[vertex].w;

                // To screen space
                clippedProjectedTriangle.vertices[vertex].x = clippedProjectedTriangle.vertices[vertex].x * screenWidth / 2.0 + screenWidth / 2.0;
                clippedProjectedTriangle.vertices[vertex].y = clippedProjectedTriangle.vertices[vertex].y * screenHeight / 2.0 + screenHeight / 2.0;
            }

            triangles.pop();

            // Backface culling
            brn::Vector3 faceNormal = clippedProjectedTriangle.calculateNormal();
            if (faceNormal.z > 0)
                continue;

            // Draw triangle
            drawFilledTriangleToPixelBuffer(clippedProjectedTriangle, lightStrength, texture);
            // drawTriangleToPixelBuffer(clippedProjectedTriangle);
        }
    }

}

void brn::BrnRenderer::drawPixelToBuffer(const Vector2i& position, const Colour& colour, float depth)
{
    // Get pixel position
    int pixel = position.x * 4 + position.y * screenWidth * 4;

    // Set pixel colour
    pixelBuffer[pixel] = colour.r;
    pixelBuffer[pixel + 1] = colour.g;
    pixelBuffer[pixel + 2] = colour.b;

    // Set depth buffer
    depthBuffer[position.x + position.y * screenWidth] = depth;
}

void brn::BrnRenderer::drawLineToPixelBuffer(const Vertex& a, const Vertex& b)
{
    int dx = (unsigned int)b.x - (unsigned int)a.x;
    int dy = (unsigned int)b.y - (unsigned int)a.y;
    int steps = std::max(std::abs(dx), std::abs(dy));
    float xInc = (float)dx / (float)steps;
    float yInc = (float)dy / (float)steps;
    float x = (unsigned int)a.x, y = (unsigned int)a.y;

    for (int i = 0; i <= steps; i++)
    {
        if (x > 0 && x < screenWidth - 1 && y > 0 && y < screenHeight - 1)
        {
            // Draw pixel to buffer
            Vector2i position = {(int)round(x), (int)round(y)};
            Colour colour = {a.r, a.g, a.b};

            drawPixelToBuffer(position, colour);
        }
        x += xInc;
        y += yInc;
    }
}

void brn::BrnRenderer::drawTriangleToPixelBuffer(const Triangle& tri)
{
    // Draw lines of triangle
    for (int i = 0; i < 3; i++)
    {
        Vertex vertexOne = tri.vertices[i];
        Vertex vertexTwo = tri.vertices[(i + 1) % 3];
        
        drawLineToPixelBuffer(vertexOne, vertexTwo);
    }
}

void brn::BrnRenderer::drawFilledTriangleToPixelBuffer(const Triangle& tri, float lightStrength, sf::Image* texture)
{
    // Get area of triangle
    int x_min = floor(std::min(std::min(tri.vertices[0].x, tri.vertices[1].x), tri.vertices[2].x));
    int x_max = floor(std::max(std::max(tri.vertices[0].x, tri.vertices[1].x), tri.vertices[2].x));
    int y_min = ceil(std::min(std::min(tri.vertices[0].y, tri.vertices[1].y), tri.vertices[2].y));
    int y_max = ceil(std::max(std::max(tri.vertices[0].y, tri.vertices[1].y), tri.vertices[2].y));

    float triangleArea = triangleEdgeCrossProduct({tri.vertices[0].x, tri.vertices[0].y}, {tri.vertices[1].x, tri.vertices[1].y},
        {tri.vertices[2].x, tri.vertices[2].y});
    
    float cross0IncX = (tri.vertices[0].y - tri.vertices[1].y);
    float cross1IncX = (tri.vertices[1].y - tri.vertices[2].y);
    float cross2IncX = (tri.vertices[2].y - tri.vertices[0].y);
    float cross0IncY = (tri.vertices[1].x - tri.vertices[0].x);
    float cross1IncY = (tri.vertices[2].x - tri.vertices[1].x);
    float cross2IncY = (tri.vertices[0].x - tri.vertices[2].x);

    float edgeBias0 = isTriangleTopOrLeftEdge({tri.vertices[1].x, tri.vertices[1].y}, {tri.vertices[2].x, tri.vertices[2].y}) ? 0 : -0.001;
    float edgeBias1 = isTriangleTopOrLeftEdge({tri.vertices[0].x, tri.vertices[0].y}, {tri.vertices[1].x, tri.vertices[1].y}) ? 0 : -0.001;
    float edgeBias2 = isTriangleTopOrLeftEdge({tri.vertices[2].x, tri.vertices[2].y}, {tri.vertices[0].x, tri.vertices[0].y}) ? 0 : -0.001;

    float cross0Y = triangleEdgeCrossProduct({tri.vertices[0].x, tri.vertices[0].y}, {tri.vertices[1].x, tri.vertices[1].y}, {(float)x_min, (float)y_min}) + edgeBias0;
    float cross1Y = triangleEdgeCrossProduct({tri.vertices[1].x, tri.vertices[1].y}, {tri.vertices[2].x, tri.vertices[2].y}, {(float)x_min, (float)y_min}) + edgeBias1;
    float cross2Y = triangleEdgeCrossProduct({tri.vertices[2].x, tri.vertices[2].y}, {tri.vertices[0].x, tri.vertices[0].y}, {(float)x_min, (float)y_min}) + edgeBias2;

    // Draw pixels in triangle
    for (int y = y_min; y < y_max; y++)
    {
        float cross0 = cross0Y;
        float cross1 = cross1Y;
        float cross2 = cross2Y;

        cross0Y += cross0IncY;
        cross1Y += cross1IncY;
        cross2Y += cross2IncY;

        if (y < 0 || y > screenHeight - 1)
            continue;
        for (int x = x_min; x < x_max; x++)
        {
            if (x < 0 || x > screenWidth - 1)
                continue;
            
            cross0 += cross0IncX;
            cross1 += cross1IncX;
            cross2 += cross2IncX;

            // If point is inside triangle, attempt to draw point
            if (cross0 >= 0 && cross1 >= 0 && cross2 >= 0)
            {
                // Interpolate vertex values using barycentric coordinates
                float pointWeight0 = cross0 / triangleArea;
                float pointWeight1 = cross1 / triangleArea;
                float pointWeight2 = cross2 / triangleArea;

                // Calculate depth of point
                float depth = pointWeight0 * tri.vertices[2].z + pointWeight1 * tri.vertices[0].z + pointWeight2 * tri.vertices[1].z;

                // If point already drawn in buffer is closer to camera, don't draw this point
                if (depthBuffer[x + y * screenWidth] < depth)
                    continue;

                // Colour calculations
                Colour colour;
                if (texture)
                {
                    // Interpolate reciprocal of depth value
                    float inverseZ = pointWeight0 * 1.0f / tri.vertices[2].w + pointWeight1 * 1.0f / tri.vertices[0].w + pointWeight2 * 1.0f / tri.vertices[1].w;

                    // Interpolate texture UV, previously perspective divided
                    // Divide by inverse Z to correct perspective based on depth
                    float u = (pointWeight0 * tri.vertices[2].u + pointWeight1 * tri.vertices[0].u + pointWeight2 * tri.vertices[1].u) / inverseZ;
                    float v = (pointWeight0 * tri.vertices[2].v + pointWeight1 * tri.vertices[0].v + pointWeight2 * tri.vertices[1].v) / inverseZ;

                    // Sample texture at UV
                    colour = sampleFromTexture(texture, u, v);
                }
                else
                {
                    // Interpolate colour
                    colour.r = pointWeight0 * tri.vertices[2].r + pointWeight1 * tri.vertices[0].r + pointWeight2 * tri.vertices[1].r;
                    colour.g = pointWeight0 * tri.vertices[2].g + pointWeight1 * tri.vertices[0].g + pointWeight2 * tri.vertices[1].g;
                    colour.b = pointWeight0 * tri.vertices[2].b + pointWeight1 * tri.vertices[0].b + pointWeight2 * tri.vertices[1].b;
                }

                // Apply lighting
                colour.r = colour.r * lightStrength;
                colour.g = colour.g * lightStrength;
                colour.b = colour.b * lightStrength;

                // Draw pixel to buffer with position, colour, and depth
                drawPixelToBuffer({x, y}, colour, depth);
            }

            
        }
    }
}

float brn::BrnRenderer::triangleEdgeCrossProduct(const Vector2& v1, const Vector2& v2, const Vector2& point)
{
    Vector2 edgeVector = {v2.x - v1.x, v2.y - v1.y};
    Vector2 pointVector = {point.x - v1.x, point.y - v1.y};

    return (edgeVector.x * pointVector.y - pointVector.x * edgeVector.y);
}

bool brn::BrnRenderer::isTriangleTopOrLeftEdge(const Vector2& v1, const Vector2& v2)
{
    Vector2 edge = {v2.x - v1.x, v2.y - v1.y};

    return (edge.y == 0 && edge.x > 0) || edge.y < 0;
}

brn::Colour brn::BrnRenderer::sampleFromTexture(sf::Image* texture, float u, float v)
{
    sf::Vector2u textureSize = texture->getSize();
    sf::Color colour = texture->getPixel((int)floor(textureSize.x * u) % textureSize.x, (int)floor(textureSize.y * v) % textureSize.y);
    return {colour.r, colour.g, colour.b};
}

void brn::BrnRenderer::setCamera(const Vector3& position, const Vector3& rotation)
{
    cameraPosition = position;
    cameraRotation = rotation;
}

void brn::BrnRenderer::setLightDirection(const Vector3& direction)
{
    float length = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    lightDirection.x = direction.x / length;
    lightDirection.y = direction.y / length;
    lightDirection.z = direction.z / length;
}

void brn::BrnRenderer::clearScreen(uint8_t r, uint8_t g, uint8_t b)
{
    window.clear();
    clearPixelBuffer(r, g, b);
    clearDepthBuffer();
}

void brn::BrnRenderer::updateScreen()
{
    sf::Image renderImage;
    renderImage.create(screenWidth, screenHeight, pixelBuffer);

    sf::Texture texture;
    texture.loadFromImage(renderImage);
    
    sf::Sprite sprite;
    sprite.setTexture(texture, true);
    sprite.setScale(sf::Vector2f(screenRenderScale, screenRenderScale));

    window.draw(sprite);

    window.display();
}

bool brn::BrnRenderer::windowOpen()
{
    return window.isOpen();
}

void brn::BrnRenderer::setWindowTitle(const std::string& title)
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
    delete [] pixelBuffer;
    pixelBuffer = nullptr;
    delete [] depthBuffer;
    depthBuffer = nullptr;
}

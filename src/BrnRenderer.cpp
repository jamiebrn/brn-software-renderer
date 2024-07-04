#include "BrnRenderer.hpp"

const std::array<brn::Plane, 6> brn::BrnRenderer::clippingPlanes = {
    Plane({-1, 0, 0}, {1, 0, 0}),
    Plane({0, 1, 0}, {0, -1, 0}),
    Plane({1, 0, 0}, {-1, 0, 0}),
    Plane({0, -1, 0}, {0, 1, 0}),
    Plane({0, 0, -1}, {0, 0, 1}),
    Plane({0, 0, 1}, {0, 0, -1})
};

brn::BrnRenderer::BrnRenderer(std::string title)
{
    // Create window
    window.create({SCREEN_WIDTH * SCREEN_RENDER_SCALE, SCREEN_HEIGHT * SCREEN_RENDER_SCALE}, title);

    // Initialise pixel and depth buffers
    pixelBuffer = new std::array<sf::Uint8, SCREEN_WIDTH * SCREEN_HEIGHT * 4>;
    depthBuffer = new std::array<float, SCREEN_WIDTH * SCREEN_HEIGHT>;

    // Reset values
    cameraPosition = {0, 0, 0};
    cameraRotation = {0, 0, 0};
    lightDirection = {0, 0, -1};

    clearScreen();
}

void brn::BrnRenderer::clearPixelBuffer(uint8_t r, uint8_t g, uint8_t b)
{
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT * 4; i += 4)
    {
        (*pixelBuffer)[i] = r;
        (*pixelBuffer)[i + 1] = g;
        (*pixelBuffer)[i + 2] = b;
        (*pixelBuffer)[i + 3] = 255;
    }
}

void brn::BrnRenderer::clearDepthBuffer()
{
    (*depthBuffer).fill(INFINITY);
}

void brn::BrnRenderer::drawMesh(const Mesh& mesh, const Vector3& position, const Vector3& rotation, const Vector3& scale)
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
            brn::vertexToClipSpace(transformedVertex, SCREEN_WIDTH, SCREEN_HEIGHT, 3.14 / 2.0, 0.1, 100);

            clipSpaceTriangle.vertices[j] = transformedVertex;
        }
        
        // Face lighting
        brn::Vector3 faceNormal = transformedTriangle.calculateNormal();

        float normalLength = sqrt(faceNormal.x * faceNormal.x + faceNormal.y * faceNormal.y + faceNormal.z * faceNormal.z);
        faceNormal = {faceNormal.x / normalLength, faceNormal.y / normalLength, faceNormal.z / normalLength};
        float dotProduct = lightDirection.x * faceNormal.x + lightDirection.y * faceNormal.y + lightDirection.z * faceNormal.z;

        // Light strength between 1 and 0.5
        float lightStrength = (dotProduct + 1) * 0.25 + 0.5;

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

                // To screen space
                clippedProjectedTriangle.vertices[vertex].x = clippedProjectedTriangle.vertices[vertex].x * SCREEN_WIDTH / 2.0 + SCREEN_WIDTH / 2.0;
                clippedProjectedTriangle.vertices[vertex].y = clippedProjectedTriangle.vertices[vertex].y * SCREEN_HEIGHT / 2.0 + SCREEN_HEIGHT / 2.0;
            }

            triangles.pop();

            // Backface culling
            brn::Vector3 faceNormal = clippedProjectedTriangle.calculateNormal();
            if (faceNormal.z > 0)
                continue;

            // Draw triangle
            drawFilledTriangleToPixelBuffer(clippedProjectedTriangle, lightStrength);
            // drawTriangleToPixelBuffer(clippedProjectedTriangle);
        }
    }

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
        if (x > 0 && x < SCREEN_WIDTH - 1 && y > 0 && y < SCREEN_HEIGHT - 1)
        {
            // Draw pixel to buffer
            int pixel = (int)(round(x) * 4) + (int)(round(y) * SCREEN_WIDTH * 4);
            (*pixelBuffer)[pixel] = a.r;
            (*pixelBuffer)[pixel + 1] = a.g;
            (*pixelBuffer)[pixel + 2] = a.b;
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

void brn::BrnRenderer::drawFilledTriangleToPixelBuffer(const Triangle& tri, float lightStrength)
{
    // Get area of triangle
    int x_min = floor(std::min(std::min(tri.vertices[0].x, tri.vertices[1].x), tri.vertices[2].x));
    int x_max = floor(std::max(std::max(tri.vertices[0].x, tri.vertices[1].x), tri.vertices[2].x));
    int y_min = ceil(std::min(std::min(tri.vertices[0].y, tri.vertices[1].y), tri.vertices[2].y));
    int y_max = ceil(std::max(std::max(tri.vertices[0].y, tri.vertices[1].y), tri.vertices[2].y));

    float triangleArea = triangleEdgeCrossProduct({tri.vertices[0].x, tri.vertices[0].y}, {tri.vertices[1].x, tri.vertices[1].y},
        {tri.vertices[2].x, tri.vertices[2].y});

    float edgeBias0 = isTriangleTopOrLeftEdge({tri.vertices[0].x, tri.vertices[0].y}, {tri.vertices[1].x, tri.vertices[1].y}) ? 0 : -1;
    float edgeBias1 = isTriangleTopOrLeftEdge({tri.vertices[1].x, tri.vertices[1].y}, {tri.vertices[2].x, tri.vertices[2].y}) ? 0 : -1;
    float edgeBias2 = isTriangleTopOrLeftEdge({tri.vertices[2].x, tri.vertices[2].y}, {tri.vertices[0].x, tri.vertices[0].y}) ? 0 : -1;

    // Draw pixels in triangle
    for (int y = y_min; y < y_max; y++)
    {
        if (y < 0 || y > SCREEN_HEIGHT - 1)
            continue;
        for (int x = x_min; x < x_max; x++)
        {
            if (x < 0 || x > SCREEN_WIDTH - 1)
                continue;
            
            // Test if pixel is in triangle using cross product
            float cross0 = triangleEdgeCrossProduct({tri.vertices[0].x, tri.vertices[0].y}, {tri.vertices[1].x, tri.vertices[1].y}, {(float)x, (float)y}) + edgeBias0;
            float cross1 = triangleEdgeCrossProduct({tri.vertices[1].x, tri.vertices[1].y}, {tri.vertices[2].x, tri.vertices[2].y}, {(float)x, (float)y}) + edgeBias1;
            float cross2 = triangleEdgeCrossProduct({tri.vertices[2].x, tri.vertices[2].y}, {tri.vertices[0].x, tri.vertices[0].y}, {(float)x, (float)y}) + edgeBias2;

            if (cross0 >= 0 && cross1 >= 0 && cross2 >= 0)
            {
                // Interpolate vertex values using barycentric coordinates
                float pointWeight0 = cross0 / triangleArea;
                float pointWeight1 = cross1 / triangleArea;
                float pointWeight2 = cross2 / triangleArea;

                float depth = pointWeight0 * tri.vertices[2].w + pointWeight1 * tri.vertices[0].w + pointWeight2 * tri.vertices[1].w;

                int colourR = pointWeight0 * tri.vertices[2].r + pointWeight1 * tri.vertices[0].r + pointWeight2 * tri.vertices[1].r;
                int colourG = pointWeight0 * tri.vertices[2].g + pointWeight1 * tri.vertices[0].g + pointWeight2 * tri.vertices[1].g;
                int colourB = pointWeight0 * tri.vertices[2].b + pointWeight1 * tri.vertices[0].b + pointWeight2 * tri.vertices[1].b;

                if ((*depthBuffer)[x + y * SCREEN_WIDTH] < depth)
                    continue;

                // Draw pixel to buffer
                int pixel = x * 4 + y * SCREEN_WIDTH * 4;
                (*pixelBuffer)[pixel] = round(colourR * lightStrength);
                (*pixelBuffer)[pixel + 1] = round(colourG * lightStrength);
                (*pixelBuffer)[pixel + 2] = round(colourB * lightStrength);
                // Draw to depth buffer
                (*depthBuffer)[x + y * SCREEN_WIDTH] = depth;
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
    delete pixelBuffer;
    pixelBuffer = nullptr;
    delete depthBuffer;
    depthBuffer = nullptr;
}

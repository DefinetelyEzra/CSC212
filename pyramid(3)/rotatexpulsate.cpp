#include <iostream>
#include <SDL.h>
#include <vector>
#include <cmath>

const int SCREEN_WIDTH = 800; // Updated width
const int SCREEN_HEIGHT = 600; // Updated height

struct Point3D {
    float x, y, z;
};

struct Edge {
    int start, end;
};

// Function to render the 3D object
void render3DObject(SDL_Renderer* renderer, const std::vector<Point3D>& points, const std::vector<Edge>& edges, float scale, float angle) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Calculate center of the pyramid
    float centerX = 0.0f;
    float centerY = 0.0f;
    float centerZ = 0.0f;

    for (const auto& point : points) {
        centerX += point.x;
        centerY += point.y;
        centerZ += point.z;
    }

    centerX /= points.size();
    centerY /= points.size();
    centerZ /= points.size();

    // Rotation matrix components
    float cosAngle = std::cos(angle);
    float sinAngle = std::sin(angle);

    // Render scaled and rotated points
    std::vector<Point3D> transformedPoints = points;
    for (auto& point : transformedPoints) {
        // Translate point to center of the pyramid
        point.x -= centerX;
        point.y -= centerY;
        point.z -= centerZ;

        // Scale point
        point.x *= scale;
        point.y *= scale;
        point.z *= scale;

        // Rotate point around Y axis
        float rotatedX = point.x * cosAngle - point.z * sinAngle;
        float rotatedZ = point.x * sinAngle + point.z * cosAngle;

        point.x = rotatedX + centerX + SCREEN_WIDTH / 2;
        point.y = point.y + SCREEN_HEIGHT / 2;
        point.z = rotatedZ + centerZ;
    }

    for (const auto& edge : edges) {
        SDL_RenderDrawLine(renderer,
            static_cast<int>(transformedPoints[edge.start].x), static_cast<int>(transformedPoints[edge.start].y),
            static_cast<int>(transformedPoints[edge.end].x), static_cast<int>(transformedPoints[edge.end].y));
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed. SDL Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create SDL window
    window = SDL_CreateWindow("3D Pyramid Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window creation failed. SDL Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create SDL renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer creation failed. SDL Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Define points and edges for the pyramid
    std::vector<Point3D> points{
        { 0, -200, 0 },   // Top point
        { -100, 200, -100 },  // Bottom left front point
        { 100, 200, -100 },   // Bottom right front point
        { 100, 200, 100 },    // Bottom right back point
        { -100, 200, 100 }    // Bottom left back point
    };

    std::vector<Edge> edges{
        { 0, 1 }, { 0, 2 },
        { 0, 3 }, { 0, 4 },
        { 1, 2 }, { 2, 3 },
        { 3, 4 }, { 4, 1 }
    };

    bool running = true;
    float scale = 1.0f;
    float pulseSpeed = 0.005f; // Decreased speed
    float angle = 0.0f;
    float rotationSpeed = 0.05f; // Increased rotation speed

    SDL_Event event;
    while (running) {
        // Event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Update scale for pulsing animation
        scale = 1.0f + 0.2f * std::sin(SDL_GetTicks() * pulseSpeed);

        // Update angle for rotation
        angle += rotationSpeed;

        render3DObject(renderer, points, edges, scale, angle);

        // Cap frame rate
        SDL_Delay(16);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
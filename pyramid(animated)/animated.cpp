#include <iostream>
#include <SDL.h>
#include <vector>
#include <cmath>

struct Point3D {
    float x, y, z;
};

struct Edge {
    int start, end;
};

// Function to render the 3D object
void render3DObject(SDL_Renderer* renderer, const std::vector<Point3D>& points, const std::vector<Edge>& edges, float scale) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Coordinates of the fixed point around which the object will pulse
    float fixedX = 400;
    float fixedY = 300;

    // Render scaled points
    std::vector<Point3D> scaledPoints = points;
    for (auto& point : scaledPoints) {
        // Calculate distance from the fixed point
        float dx = point.x - fixedX;
        float dy = point.y - fixedY;

        // Scale each point around the fixed point
        point.x = fixedX + scale * dx;
        point.y = fixedY + scale * dy;
        point.z *= scale;
    }

    for (const auto& edge : edges) {
        SDL_RenderDrawLine(renderer,
            static_cast<int>(scaledPoints[edge.start].x), static_cast<int>(scaledPoints[edge.start].y),
            static_cast<int>(scaledPoints[edge.end].x), static_cast<int>(scaledPoints[edge.end].y));
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    const int SCREEN_WIDTH = 800; // Updated width
    const int SCREEN_HEIGHT = 600; // Updated height

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
        { 400, 100, 100 },   // Top point
        { 200, 500, -100 },  // Bottom left point
        { 600, 500, -100 },  // Bottom right point
        { 400, 500, 300 }    // Back point
    };

    std::vector<Edge> edges{
        { 0, 1 }, { 0, 2 },
        { 0, 3 }, { 1, 2 },
        { 2, 3 }, { 3, 1 }
    };

    bool running = true;
    float scale = 1.0f;
    float pulseSpeed = 0.005f; // Decreased speed

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

        render3DObject(renderer, points, edges, scale);

        // Cap frame rate
        SDL_Delay(16);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
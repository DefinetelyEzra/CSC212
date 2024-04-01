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

void render3DObject(SDL_Renderer* renderer, const std::vector<Point3D>& points, const std::vector<Edge>& edges) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (const auto& edge : edges) {
        SDL_RenderDrawLine(renderer,
            static_cast<int>(points[edge.start].x), static_cast<int>(points[edge.start].y),
            static_cast<int>(points[edge.end].x), static_cast<int>(points[edge.end].y));
    }

    SDL_RenderPresent(renderer);
}

void rotatePoints(std::vector<Point3D>& points, float angle, const Point3D& center) {
    float cosAngle = std::cos(angle);
    float sinAngle = std::sin(angle);

    for (auto& point : points) {
        float translatedX = point.x - center.x;
        float translatedZ = point.z - center.z;

        float rotatedX = translatedX * cosAngle - translatedZ * sinAngle;
        float rotatedZ = translatedX * sinAngle + translatedZ * cosAngle;

        point.x = rotatedX + center.x;
        point.z = rotatedZ + center.z;
    }
}

int main(int argc, char* argv[]) {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

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
        { 400, 150, 0 },   // Top point
        { 300, 450, -100 }, // Bottom left point
        { 500, 450, -100 }, // Bottom right point
        { 300, 450, 100 }, // Back left point
        { 500, 450, 100 }  // Back right point
    };

    std::vector<Edge> edges{
        { 0, 1 }, { 0, 2 },
        { 0, 3 }, { 0, 4 }, 
        { 1, 2 }, { 2, 4 }, 
        { 4, 3 }, { 3, 1 } 
    };

    Point3D center = { 400, 450, 0 }; // Centered on the bottom plane of the pyramid

    bool running = true;
    const float rotationSpeed = 0.01f; 

    SDL_Event event;
    while (running) {
        // Event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        rotatePoints(points, rotationSpeed, center);

        render3DObject(renderer, points, edges);

        // Cap frame rate
        SDL_Delay(16);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
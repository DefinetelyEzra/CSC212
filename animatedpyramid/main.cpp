#include <iostream>
#include <vector>
#include <chrono>
#include <SDL.h>
#include <cmath>

struct Point2D { float x; float y; };
struct Point3D { float x; float y; float z; };

int WindowSizeX;
int WindowSizeY;
SDL_Renderer* renderer;
SDL_Window* window;
float FL = 5;
float DeltaTime;
float scaledValue = 2;

int iterations = 0;

// Vertices of the pyramid
std::vector<Point3D> points2{
    Point3D{0, 1, 0},    // Top point (apex)
    Point3D{-1, -1, 1},  // Front bottom-left
    Point3D{1, -1, 1},   // Front bottom-right
    Point3D{1, -1, -1},  // Back bottom-right
    Point3D{-1, -1, -1}  // Back bottom-left
};

// Faces of the pyramid (indices referencing the points array)
std::vector<std::vector<int>> faces{
    {0, 1, 2},  // Front face
    {0, 2, 3},  // Right face
    {0, 3, 4},  // Back face
    {0, 4, 1},  // Left face
    {1, 2, 3, 4}  // Bottom face
};

Point2D projection2(Point3D point)
{
    return Point2D{ WindowSizeX / 2 + (point.x * FL) / (FL + point.z) * 100, WindowSizeY / 2 - (point.y * FL) / (FL + point.z) * 100 };
}

Point3D scaleOut(Point3D point, float scale)
{
    Point3D scaledPoint;
    scaledPoint.x = point.x * scale;
    scaledPoint.y = point.y * scale;
    scaledPoint.z = point.z * scale;
    return scaledPoint;
}

void render2(std::vector<Point3D> points, std::vector<std::vector<int>> faces)
{
    iterations++;
    auto time1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration(0);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    // Adjust the scale periodically for pulsating effect
    float pulsationSpeed = 0.002; // Adjust the speed as needed (lower value for slower pulsation)
    float scale = 1 + 0.5 * sin(iterations * pulsationSpeed);

    // Apply transformations and projection for each face
    for (auto& face : faces) {
        // Project and draw the face
        std::vector<Point2D> projectedPoints;
        for (int i : face) {
            // Scale the points
            Point3D scaledPoint = scaleOut(points[i], scale);
            // Project the points to 2D
            projectedPoints.push_back(projection2(scaledPoint));
        }

        // Draw the lines to form the face
        for (int i = 0; i < projectedPoints.size(); ++i) {
            int j = (i + 1) % projectedPoints.size(); // Wrap around to connect the last point to the first
            SDL_RenderDrawLine(renderer, projectedPoints[i].x, projectedPoints[i].y, projectedPoints[j].x, projectedPoints[j].y);
        }
    }

    SDL_RenderPresent(renderer);
    auto time2 = std::chrono::high_resolution_clock::now();
    duration = time2 - time1;
    DeltaTime = duration.count();
    time1 = time2;
}

// Default C++ function
int main(int argc, char* argv[])
{
    iterations = 0;
    // Creates a centered window with 960 width and 540 height
    window = SDL_CreateWindow("Cube Renderer No OOP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_GetWindowSize(window, &WindowSizeX, &WindowSizeY);

    bool running = true;

    while (running)
    {
        // If the quit button is pressed, the loop breaks
        if (SDL_QuitRequested()) { running = false; break; }

        // Calls the render function
        render2(points2, faces);
    }

    return 0;
}
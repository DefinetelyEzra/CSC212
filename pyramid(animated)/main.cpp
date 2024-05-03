#include <iostream>
#include <SDL.h>
#include <vector>
#include <cmath>
#include "Renderer.h"
#include "Point3D.h"
#include "Edge.h"

int main(int argc, char* argv[]) {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT);
    renderer.initializeSDL();

    std::vector<Point3D> points{
        { 400, 100, 100 },
        { 200, 500, -100 },
        { 600, 500, -100 },
        { 400, 500, 300 }
    };

    std::vector<Edge> edges{
        { 0, 1 }, { 0, 2 },
        { 0, 3 }, { 1, 2 },
        { 2, 3 }, { 3, 1 }
    };

    renderer.setPointsAndEdges(points, edges);

    bool running = true;
    float scale = 1.0f;
    float pulseSpeed = 0.005f;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        scale = 1.0f + 0.2f * std::sin(SDL_GetTicks() * pulseSpeed);
        renderer.render3DObject(scale);
        SDL_Delay(16);
    }

    renderer.cleanup();
    return 0;
}
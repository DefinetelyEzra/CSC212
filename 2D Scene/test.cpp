#include "test.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int IMAGE_WIDTH = SCREEN_WIDTH + 300;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
int speed = 0.1;
int CameraX = 0;
int textureWidthDiff = IMAGE_WIDTH - SCREEN_WIDTH;

class Snowflake {
public:
    int x, y;
    int speed;

    Snowflake(int posX, int posY, int spd) : x(posX), y(posY), speed(spd) {}
    void update() {
        y += speed;
        if (y > SCREEN_HEIGHT)
            y = 0;
    }
};

class Ball {
public:
    int x, y;
    int velocity;

    Ball(int posX, int posY, int vel) : x(posX), y(posY), velocity(vel) {}
    void update() {
        x += velocity;
        if (x > SCREEN_WIDTH + 50) // Reset position when ball goes off-screen
            x = -50;
    }
};

SDL_Texture* loadTexture(const std::string& path) {
    // The final texture
    SDL_Texture* newTexture = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

bool initSDL()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create window
    gWindow = SDL_CreateWindow("2D Rendering", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Set renderer color
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Allows for (Initializes) Image Loading abilities using SDL_image library
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void closeSDL()
{
    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!initSDL()) {
        printf("Failed to initialize SDL!\n");
        return -1;
    }

    std::vector<SDL_Texture*> backgroundTextures;
    backgroundTextures.push_back(loadTexture("./Assets/Pixel_Forest/layer101.png"));
    backgroundTextures.push_back(loadTexture("./Assets/Pixel_Forest/layer102.png"));
    backgroundTextures.push_back(loadTexture("./Assets/Pixel_Forest/layer103.png"));

    SDL_Texture* backgroundFloor = loadTexture("./Assets/grassfloor.png");
    SDL_Texture* backgroundSky = loadTexture("./Assets/skytoon.jpg");
    SDL_Texture* snowflakeTexture = loadTexture("./Assets/snowflake.png");
    SDL_Texture* ballTexture = loadTexture("./Assets/ball.png");

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    // Seed random number generator
    srand(time(NULL));

    std::vector<Snowflake> snowflakes;
    for (int i = 0; i < 100; ++i) {
        int x = rand() % SCREEN_WIDTH;
        int y = rand() % SCREEN_HEIGHT;
        int speed = rand() % 3 + 1; // Random speed between 1 and 3
        snowflakes.push_back(Snowflake(x, y, speed));
    }

    std::vector<Ball> balls;
    balls.push_back(Ball(800, SCREEN_HEIGHT - 155, 4)); // Create a ball that starts at the left edge of the screen

    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear screen
        SDL_RenderClear(gRenderer);

        // Render sky and floor
        SDL_Rect skyDest = { 0, -400, 800, 810 };
        SDL_RenderCopy(gRenderer, backgroundSky, NULL, &skyDest);
        SDL_RenderCopy(gRenderer, backgroundFloor, NULL, NULL);

        // Render background layers
        for (SDL_Texture* texture : backgroundTextures) {
            SDL_Rect textureDest = { -140, 0, IMAGE_WIDTH, SCREEN_HEIGHT };
            SDL_RenderCopy(gRenderer, texture, NULL, &textureDest);
        }

        // Update and render snowflakes
        for (auto& snowflake : snowflakes) {
            snowflake.update();
            SDL_Rect snowflakeDest = { snowflake.x, snowflake.y, 7.5, 7.5 }; // Adjust size as needed
            SDL_RenderCopy(gRenderer, snowflakeTexture, NULL, &snowflakeDest);
        }

        // Update and render balls
        for (auto& ball : balls) {
            ball.update();
            SDL_Rect ballDest = { ball.x, ball.y, 40, 40 }; // Adjust size as needed
            SDL_RenderCopy(gRenderer, ballTexture, NULL, &ballDest);
        }

        // Update screen
        SDL_RenderPresent(gRenderer);

        // Add a small delay to control the frame rate
        SDL_Delay(10); // Adjust as needed for desired frame rate
    }

    // Destroy textures
    SDL_DestroyTexture(backgroundFloor);
    SDL_DestroyTexture(backgroundSky);
    SDL_DestroyTexture(snowflakeTexture);
    SDL_DestroyTexture(ballTexture);

    for (SDL_Texture* texture : backgroundTextures) {
        SDL_DestroyTexture(texture);
    }

    // Close SDL
    closeSDL();

    return 0;
}
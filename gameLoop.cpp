#include <iostream>
#include <utility>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "gameLoop.h"
using namespace std;

GameLoop::GameLoop(string windowName, const int& width, const int& height) : windowName(move(windowName)), width(width), height(height) {
    mainWindow = nullptr;
    mainRenderer = nullptr;
    mainTexture = nullptr;

    try {
        init();
    } catch (SDLImgException& e) {
        cerr << e.what() << endl;
        exit(1);
    } catch(SDLException& e) {
        cerr << e.what() << endl;
        exit(1);
    }

    mainLoop();
}

void GameLoop::init() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw SDLException("SDL could not initialize!");
    }

    // Set texture filtering to linear
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        cerr << "Warning: Linear texture filtering not enabled!" << endl;
    }

    // Create a window
    mainWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN); // NOLINT(hicpp-signed-bitwise)
    if (mainWindow == nullptr) {
        throw SDLException("The main window could not be created!");
    }

    // Create renderer for the window
    mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
    if (mainRenderer == nullptr) {
        throw SDLException("The renderer could not be created!");
    }

    // Initialize renderer colour
    SDL_SetRenderDrawColor(mainRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Initialize PNG loading
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) { // NOLINT(hicpp-signed-bitwise)
        throw SDLImgException("SDL_image could not be initialized!");
    }
}

SDL_Texture* GameLoop::loadTexture(const string &path) const {
    // Final texture
    SDL_Texture* newTexture = nullptr;

    // Load image
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        throw SDLImgException("Unable to load image '" + path + "'");
    }

    // Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(mainRenderer, loadedSurface);
    if (newTexture == nullptr) {
        throw SDLException("Unable to create texture from '" + path + "'");
    }

    // Free up loaded surface
    SDL_FreeSurface(loadedSurface);

    return newTexture;
}

void GameLoop::loadMedia() {

}

void GameLoop::mainLoop() {
    bool quit = false;  // Main loop flag
    SDL_Event event;    // Event handler

    int recX = (width * 7) / 16;
    int recY = (height * 7) / 16;

    while (!quit) {
        // Handle events on the queue
        while (SDL_PollEvent(&event) != 0) {
            // Exit request from user
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_KEYDOWN) { // User presses a key
                // Select surface based on key
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        if (recY > 0) {
                            recY -= 5;
                        }
                        break;

                    case SDLK_DOWN:
                        if (recY < (height - (height / 8))) {
                            recY += 5;
                        }
                        break;

                    case SDLK_LEFT:
                        if (recX > 0) {
                            recX -= 5;
                        }
                        break;

                    case SDLK_RIGHT:
                        if (recX < (width - (width / 8))) {
                            recX += 5;
                        }
                        break;

                }
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(mainRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(mainRenderer);

        // Render red filled quad
        SDL_Rect fillRect = { recX, recY, width / 8, height / 8 };
        SDL_SetRenderDrawColor(mainRenderer, 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(mainRenderer, &fillRect);

        // Update screen
        SDL_RenderPresent(mainRenderer);
    }
}

void GameLoop::close() {
    // Deallocate surfaces
    SDL_DestroyTexture(mainTexture);
    mainTexture = nullptr;

    // Destroy window
    SDL_DestroyRenderer(mainRenderer);
    SDL_DestroyWindow(mainWindow);
    mainWindow = nullptr;
    mainRenderer = nullptr;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

SDLException::SDLException(string msg): errMsg(move(msg)) {}
string& SDLException::what() {
    errMsg += "\nSDL Error: " + static_cast<string>(SDL_GetError());
    return errMsg;
}

SDLImgException::SDLImgException(string msg): errMsg(move(msg)) {}
string& SDLImgException::what() {
    errMsg += "\nSDL_image Error: " + static_cast<string>(IMG_GetError());
    return errMsg;
}

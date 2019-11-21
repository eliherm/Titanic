#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <utility>
#include <vector>
#include "view.h"
using namespace std;

gameDisplay::gameDisplay(): WIDTH(0), HEIGHT(0), window(nullptr), renderer(nullptr) {}
gameDisplay::gameDisplay(const string& windowName, const int& height, const int& width) {
    WIDTH = width;
    HEIGHT = height;
    window = nullptr;
    renderer = nullptr;

    try {
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw SDLException("SDL could not initialize!");
        }

        // Set texture filtering to linear
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            cerr << "Warning: Linear texture filtering not enabled!" << endl;
        }

        // Create a window
        window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN); // NOLINT(hicpp-signed-bitwise)
        if (window == nullptr) {
            throw SDLException("The main window could not be created!");
        }

        // Create renderer for the window
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr) {
            throw SDLException("The renderer could not be created!");
        }

        // Initialize renderer colour
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Initialize PNG loading
        int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
        if (!(IMG_Init(imgFlags) & imgFlags)) { // NOLINT(hicpp-signed-bitwise)
            throw SDLImgException("SDL_image could not be initialized!");
        }
    } catch (SDLImgException& e) {
        cerr << e.what() << endl;
        exit(1);
    } catch(SDLException& e) {
        cerr << e.what() << endl;
        exit(1);
    }
}

void gameDisplay::levelInit(const int& doorX, const int& doorY) {
	// Setting player, water, and door dimensions
	player.setDim(128, 240);
    water.setDim(WIDTH, HEIGHT);
    door.setDim(40, 80);

	// Setting door position
	door.setPos(doorX, doorY);

    // Initialize player textures
    player.spriteSheet = new TextureWrap(renderer, "../titanic_eclipse/assets/player.png");
    SDL_Rect playerFrontClip = {32, 24, 128, 240};  // Front
    SDL_Rect playerLeftClip = {552, 288, 104, 240}; // Left
    SDL_Rect playerRightClip = {552, 24, 104, 240}; // Right
    SDL_Rect playerBackClip = {32, 288, 128, 240};  // Back
    player.spriteClips.push_back(playerFrontClip);
    player.spriteClips.push_back(playerLeftClip);
    player.spriteClips.push_back(playerRightClip);
    player.spriteClips.push_back(playerBackClip);

    // Initialize water textures
    water.spriteSheet = new TextureWrap(renderer, "../titanic_eclipse/assets/water.png");

    // Initialize door textures
    door.spriteSheet = new TextureWrap(renderer, "../titanic_eclipse/assets/door.png");
    SDL_Rect doorClip = {193, 384, 46, 95};
    door.spriteClips.push_back(doorClip);
}

void gameDisplay::update(vector<object> objects, bool win, bool lose) {
	// update objects
	player.setPos(objects.at(0).getXCoord(), objects.at(0).getYCoord());
	water.setPos(0, objects.at(2).getYCoord());

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

	// Render water
    water.spriteSheet->render(0, water.getYPos(), nullptr);

    // Render door
    door.spriteSheet->render(door.getXPos(), door.getYPos(), &(door.spriteClips.at(0)));

	// Draw platforms
	SDL_SetRenderDrawColor(renderer, 0xc2, 0xc5, 0xcc, 0xff);
	for (int i = 3; i < objects.size(); i++) {
		SDL_Rect platform = {objects.at(i).getXCoord(), objects.at(i).getYCoord(), objects.at(i).getWidth(), objects.at(i).getHeight()};
		SDL_RenderFillRect(renderer, &platform);
	}

	// Render player
    player.spriteSheet->render(player.getXPos(), player.getYPos(), &(player.spriteClips.at(0)));

	// Dumping buffer to screen
	SDL_RenderPresent(renderer);
}

void gameDisplay::close() {
    // Free textures
    delete player.spriteSheet;
    delete water.spriteSheet;
    delete door.spriteSheet;
    player.spriteSheet = nullptr;
    water.spriteSheet = nullptr;
    door.spriteSheet = nullptr;

	// Destroying everything!
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = nullptr;
	window = nullptr;

	// Quit SDL subsystems
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

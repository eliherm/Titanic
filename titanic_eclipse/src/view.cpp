#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <utility>
#include <vector>
#include "view.h"
using namespace std;

sprite::sprite() {
	this->xcoord = 0;
	this->ycoord = 0;
	this->width = 0;
	this->height = 0;
}

sprite::sprite(const int &xpos, const int &ypos, const int &width, const int &height) {
	this->xcoord = xpos;
	this->ycoord = ypos;
	this->width = width;
	this->height = height;
}

void sprite::setDim(const int& width, const int& height) {
	this->width = width;
	this->height = height;
}

void sprite::setPos(const int &xpos, const int &ypos) {
	xcoord = xpos;
	ycoord = ypos;
}

// Sprite Accessors
int sprite::getWidth() const { return this->width; }
int sprite::getHeight() const { return this->height; }
int sprite::getXPos() const { return this->xcoord; }
int sprite::getYPos() const { return this->ycoord; }

gameDisplay::gameDisplay() {
    WIDTH = 0;
    HEIGHT = 0;
    window = nullptr;
    renderer = nullptr;
}

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
	player.setDim(40, 80);
	water.setDim(WIDTH, HEIGHT);
	door.setDim(40, 80);

	// Setting door position
	door.setPos(doorX, doorY);
}

void gameDisplay::update(vector<object> objects, bool win, bool lose) {
	// update objects
	player.setPos(objects.at(0).getXCoord(), objects.at(0).getYCoord());
	water.setPos(0, objects.at(2).getYCoord());

	// Draw background
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	// Draw water
	SDL_Rect waterRect = {0, water.getYPos(), water.getWidth(), water.getHeight()};
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xff, 0xff);
	SDL_RenderFillRect(renderer, &waterRect);

    // Draw door
    SDL_Rect doorRect = {door.getXPos(), door.getYPos(), door.getWidth(), door.getHeight()};
    SDL_SetRenderDrawColor(renderer, 0xd2, 0xb4, 0x8c, 0xff);
    SDL_RenderFillRect(renderer, &doorRect);

	// Draw platforms
	SDL_SetRenderDrawColor(renderer, 0xc2, 0xc5, 0xcc, 0xff);
	for (int i = 3; i < objects.size(); i++) {
		SDL_Rect platform = {objects.at(i).getXCoord(), objects.at(i).getYCoord(), objects.at(i).getWidth(), objects.at(i).getHeight()};
		SDL_RenderFillRect(renderer, &platform);
	}

	// Draw player
	SDL_Rect playerRect = {player.getXPos(), player.getYPos(), player.getWidth(), player.getHeight()};
	SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
	SDL_RenderFillRect(renderer, &playerRect);

	// Dumping buffer to screen
	SDL_RenderPresent(renderer);
}

void gameDisplay::close() {
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

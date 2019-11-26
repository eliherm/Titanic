#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <utility>
#include <vector>
#include "view.h"
#include <iostream>
using namespace std;

#define ANIMATION_DELAY 10   // Controls how fast frames are rendered in an animation cycle

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
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
	camera = {0, player.getYPos() , WIDTH, HEIGHT };
    water.setDim(WIDTH, HEIGHT);
    door.setDim(40, 80);

	// Setting door position
	door.setPos(doorX, doorY);

    // Initialize player textures
    player.spriteSheet = new TextureWrap(renderer, "../titanic_eclipse/assets/player.png");

    /*
     * Initialize animation parameters for the player
     * playerFrames[0][x] -> Current frame for a given direction x
     * playerFrames[1][x] -> Total number of frames for a given direction x
     */
    vector<vector<int>> playerFrames { {0, 0, 0}, {1, 3, 3} };
    player.enableAnimation(playerFrames);

    // Set sprite clips for the player
    SDL_Rect playerFrontClip = {32, 24, 128, 240};      // Front sprite
    SDL_Rect playerLeftClip1 = {552, 288, 104, 240};    // Left sprite (frame 1)
    SDL_Rect playerLeftClip2 = {680, 288, 120, 240};    // Left sprite (frame 2)
    SDL_Rect playerLeftClip3 = {816, 288, 120, 240};    // Left sprite (frame 3)

    SDL_Rect playerRightClip1 = {552, 24, 104, 240};    // Right sprite (frame 1)
    SDL_Rect playerRightClip2 = {688, 24, 120, 240};    // Right sprite (frame 2)
    SDL_Rect playerRightClip3 = {840, 24, 120, 240};    // Right sprite (frame 3)
    SDL_Rect playerBackClip = {32, 288, 128, 240};      // Back sprite

    player.spriteClips.push_back(playerFrontClip);
    player.spriteClips.push_back(playerLeftClip1);
    player.spriteClips.push_back(playerLeftClip2);
    player.spriteClips.push_back(playerLeftClip3);
    player.spriteClips.push_back(playerRightClip1);
    player.spriteClips.push_back(playerRightClip2);
    player.spriteClips.push_back(playerRightClip3);
    player.spriteClips.push_back(playerBackClip);

    // Initialize water textures
    water.spriteSheet = new TextureWrap(renderer, "../titanic_eclipse/assets/water.png");

    // Initialize door textures
    door.spriteSheet = new TextureWrap(renderer, "../titanic_eclipse/assets/door.png");
    SDL_Rect doorClip = {193, 384, 46, 95};
    door.spriteClips.push_back(doorClip);

    // Initialize platforms
    platforms.spriteSheet = new TextureWrap(renderer, "../titanic_eclipse/assets/industrial.v1.png");
    SDL_Rect platform1 = {192, 0, 64, 19};
    platforms.spriteClips.push_back(platform1);
}

void gameDisplay::update(vector<object> objects, vector<bool> keys, bool win, bool lose, bool grounded) {
	// update objects
	player.setPos(objects.at(0).getXCoord(), objects.at(0).getYCoord());

	// update camera location
	camera.y = player.getYPos();

	water.setPos(0, objects.at(2).getYCoord());

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    // Render door
    if (door.getYPos() >= 0 and door.getYPos() <= camera.y + HEIGHT) { //this condition checks if the door is within the view of the player
        door.spriteSheet->render(door.getXPos(), door.getYPos(), &(door.spriteClips.at(0)));
    }

	// Draw platforms
	for (int i = 3; i < objects.size(); i++) {
        if (objects.at(i).getYCoord() >= 0 and objects.at(i).getYCoord() <= camera.y + HEIGHT) { //this condition checks if the platforms are within the view of the player
            platforms.spriteSheet->render(objects.at(i).getXCoord(), objects.at(i).getYCoord(),
                                          &(platforms.spriteClips.at(0)));
        }
	}

    // Render player with animation

	//LEFT
    if(keys[2] && !keys[3]) {
        // Reset frame information for other directions
        for (auto i : player.frames[0]) {
            if (i != 1) i = 0;
        }

        // Render the frame
        if (grounded) {
            player.spriteSheet->render(player.getXPos(), player.getYPos(),
                                       &(player.spriteClips.at(1 + (player.frames[0][1] / ANIMATION_DELAY))));

            // Increment frame count for left direction
            player.frames[0][1]++;
            if (player.frames[0][1] / ANIMATION_DELAY >= player.frames[1][1])
                player.frames[0][1] = 0;
        } else { //this is for when the player is moving left but in the air, if we want to render a different animation

            player.spriteSheet->render(player.getXPos(), player.getYPos(), &(player.spriteClips.at(0)));
        }

        //RIGHT
    } else if(keys[3] && !keys[2]){
        // Reset frame information for other directions
        for (auto i : player.frames[0]) {
            if (i != 2) i = 0;
        }

        if (grounded) {
            // Render the frame
            player.spriteSheet->render(player.getXPos(), player.getYPos(),
                                       &(player.spriteClips.at(4 + (player.frames[0][2] / ANIMATION_DELAY))));

            // Increment frame count for right direction
            player.frames[0][2]++;
            if (player.frames[0][2] / ANIMATION_DELAY >= player.frames[1][2])
                player.frames[0][2] = 0;
        } else { //this is for when the player is moving right but in the air, if we want to render a different animation
            player.spriteSheet->render(player.getXPos(), player.getYPos(), &(player.spriteClips.at(0)));
        }
        //NOT MOVING
    } else {
         player.spriteSheet->render(player.getXPos(), player.getYPos(), &(player.spriteClips.at(0)));
    }

    // Render water
    if (water.getYPos() >= 0 and water.getYPos() <= camera.y + HEIGHT) //this condition checks if the water is in the view of the player
        water.spriteSheet->render(0, water.getYPos(), nullptr);

	// Dumping buffer to screen
	SDL_RenderPresent(renderer);
}

void gameDisplay::close() {
    // Free textures
    delete player.spriteSheet;
    delete water.spriteSheet;
    delete door.spriteSheet;
    delete platforms.spriteSheet;
    player.spriteSheet = nullptr;
    water.spriteSheet = nullptr;
    door.spriteSheet = nullptr;
    platforms.spriteSheet = nullptr;

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

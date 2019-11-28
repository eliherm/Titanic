#define WINDOWS   // Define the platform

#include <SDL2/SDL.h>
#include <iostream>
#include <utility>
#include <vector>
#include "view.h"
#include <iostream>
// Include the SDL image header based on the platform
#ifdef WINDOWS
    #include <SDL2/SDL_image.h>
#else
    #include <SDL_image.h>
#endif
using namespace std;

#define ANIMATION_DELAY 10  // Controls how fast frames are rendered in an animation cycle

gameDisplay::gameDisplay(): WIDTH(0), HEIGHT(0), window(nullptr), renderer(nullptr), camera({0, 0, WIDTH, HEIGHT}) {}
gameDisplay::gameDisplay(const string& windowName, const int& width, const int& height) {
    WIDTH = width;
    HEIGHT = height;
    window = nullptr;
    renderer = nullptr;
    camera = {0, 0, WIDTH, HEIGHT};

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
    water.setDim(WIDTH, HEIGHT);
    door.setDim(40, 80);

	// Setting door position
	door.setPos(doorX, doorY);

	// Initialize the paths to textures
#ifdef WINDOWS
    string playerTexPath = "..\\titanic\\titanic_eclipse\\assets\\player.png";
    string waterTexPath = "..\\titanic\\titanic_eclipse\\assets\\water.png";
    string doorTexPath = "..\\titanic\\titanic_eclipse\\assets\\door.png";
    string platformsTexPath = "..\\titanic\\titanic_eclipse\\assets\\platform.png";
#else
    string playerTexPath = "../titanic_eclipse/assets/player.png";
    string waterTexPath = "../titanic_eclipse/assets/water.png";
    string doorTexPath = "../titanic_eclipse/assets/door.png";
    string platformsTexPath = "../titanic_eclipse/assets/platform.png";
#endif

    // Initialize player textures
    player.spriteSheet = new TextureWrap(renderer, playerTexPath);

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
    water.spriteSheet = new TextureWrap(renderer, waterTexPath);

    // Initialize door textures
    door.spriteSheet = new TextureWrap(renderer, doorTexPath);
    SDL_Rect doorClip = {193, 384, 46, 95};
    door.spriteClips.push_back(doorClip);

    // Initialize platforms
    platforms.spriteSheet = new TextureWrap(renderer, platformsTexPath);
//    SDL_Rect platform1 = {192, 0, 64, 19};
//    platforms.spriteClips.push_back(platform1);
}

void gameDisplay::update(vector<object> objects, vector<bool> keys, bool grounded, bool win, bool lose) {
	// Update objects
	player.setPos(objects.at(0).getXCoord(), objects.at(0).getYCoord());
    water.setPos(0, objects.at(2).getYCoord());

	// Update camera location to the center of the player in the y-axis
	camera.y = (player.getYPos() + player.getHeight() / 2) - HEIGHT / 2;

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    // Render door
    door.spriteSheet->render(door.getXPos(), door.getYPos() - camera.y, &(door.spriteClips.at(0)));

	// Draw platforms
	for (int i = 3; i < objects.size(); i++) {
        platforms.spriteSheet->render(objects.at(i).getXCoord(), objects.at(i).getYCoord() - camera.y, nullptr);
	}

    // Render player with animation

	//LEFT
    if (keys[2] && !keys[3] && grounded) { // Left and on the ground
        // Reset frame information for other directions
        for (auto i : player.frames[0])
            if (i != 1) i = 0;

        // Render the frame
        player.spriteSheet->render(player.getXPos(), player.getYPos() - camera.y,
                                   &(player.spriteClips.at(1 + (player.frames[0][1] / ANIMATION_DELAY))));

        // Increment frame count for left direction
        player.frames[0][1]++;
        if (player.frames[0][1] / ANIMATION_DELAY >= player.frames[1][1])
            player.frames[0][1] = 0;
    } else if (keys[2] && !keys[3] && !grounded) { //left and in the air
    	player.spriteSheet->render(player.getXPos(), player.getYPos() - camera.y, &(player.spriteClips.at(7))); //make this the sprite for in the air moving left
    }

    //RIGHT
	else if (keys[3] && !keys[2] && grounded) { // Right and on the ground
        // Reset frame information for other directions
        for (auto i : player.frames[0])
            if (i != 2) i = 0;

        // Render the frame
        player.spriteSheet->render(player.getXPos(), player.getYPos() - camera.y,
                                   &(player.spriteClips.at(4 + (player.frames[0][2] / ANIMATION_DELAY))));

        // Increment frame count for right direction
        player.frames[0][2]++;
        if (player.frames[0][2] / ANIMATION_DELAY >= player.frames[1][2])
            player.frames[0][2] = 0;
    } else if (keys[3] && !keys[2] && !grounded){ //Right and in the air
    	player.spriteSheet->render(player.getXPos(), player.getYPos() - camera.y, &(player.spriteClips.at(7))); //make this the sprite for in the air moving right
    }

    //STANDING STILL
    else {
         player.spriteSheet->render(player.getXPos(), player.getYPos() - camera.y, &(player.spriteClips.at(0)));
    }

    // Render water
    water.spriteSheet->render(0, water.getYPos() - camera.y, nullptr);

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

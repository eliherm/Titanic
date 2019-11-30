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

gameDisplay::gameDisplay(): WIDTH(0), HEIGHT(0), window(nullptr),
		renderer(nullptr), camera({0, 0, WIDTH, HEIGHT}), menuState(start), optionSelected(0) {}

gameDisplay::gameDisplay(const string& windowName, const int& width, const int& height) {
    WIDTH = width;
    HEIGHT = height;
    window = nullptr;
    renderer = nullptr;
    camera = {0, 0, WIDTH, HEIGHT};
    menuState = start;
    optionSelected = 0;

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

    	initTextures();
    } catch (SDLImgException& e) {
        cerr << e.what() << endl;
        exit(1);
    } catch(SDLException& e) {
        cerr << e.what() << endl;
        exit(1);
    }
}

void gameDisplay::initTextures() {
    // Initialize the paths to textures
#ifdef WINDOWS
    string playerTexPath = "..\\assets\\sprites\\player.png";
    string waterTexPath = "..\\assets\\sprites\\water.png";
    string doorTexPath = "..\\assets\\sprites\\door.png";
    string platformsTexPath = "..\\assets\\sprites\\platform.png";
    string loseMenuImg = "..\\assets\\sprites\\menus\\lose-menu.png";
    string pauseMenuImg = "..\\assets\\sprites\\menus\\pause-menu.png";
    string startMenuImg = "..\\assets\\sprites\\menus\\start-menu.png";
    string winMenuImg = "..\\assets\\sprites\\menus\\win-menu.png";
#else
    string playerTexPath = "../assets/sprites/player.png";
    string waterTexPath = "../assets/sprites/water.png";
    string doorTexPath = "../assets/sprites/door.png";
    string platformsTexPath = "../assets/sprites/platform.png";
    string loseMenuImg = "../assets/sprites/menus/lose-menu.png";
    string pauseMenuImg = "../assets/sprites/menus/pause-menu.png";
    string startMenuImg = "../assets/sprites/menus/start-menu.png";
    string winMenuImg = "../assets/sprites/menus/win-menu.png";
#endif

    // Setting player, water, and door dimensions
    player.setDim(60, 128);
    water.setDim(WIDTH, HEIGHT);
    door.setDim(40, 80);

    // Initialize player textures
    player.spriteSheet = new TextureWrap(renderer, playerTexPath);

    /*
     * Initialize animation parameters for the player
     * playerFrames[0][x] -> Current frame for a given direction x
     * playerFrames[1][x] -> Total number of frames for a given direction x
     */

    vector<vector<int>> playerFrames { {0, 0, 0}, {1, 4, 4} };
    player.enableAnimation(playerFrames);

    // Set sprite clips for the player
    SDL_Rect playerFrontClip = {29, 0, 70, 128};      // Front sprite
    SDL_Rect playerLeftClip1 = {42, 256, 44, 128};    // Left sprite (frame 1)
    SDL_Rect playerLeftClip2 = {169, 257, 60, 126};    // Left sprite (frame 2)
    SDL_Rect playerLeftClip3 = {302, 257, 44, 129};    // Left sprite (frame 2)
    SDL_Rect playerLeftClip4 = {426, 258, 60, 126};    // Left sprite (frame 3)

    SDL_Rect playerRightClip1 = {40, 128, 44, 128};    // Right sprite (frame 1)
    SDL_Rect playerRightClip2 = {166, 129, 60, 128};    // Right sprite (frame 2)
    SDL_Rect playerRightClip3 = {303, 128, 45, 128};    // Right sprite (frame 2)
    SDL_Rect playerRightClip4 = {423, 130, 60, 126};    // Right sprite (frame 3)

    player.spriteClips.push_back(playerFrontClip);
    player.spriteClips.push_back(playerLeftClip1);
    player.spriteClips.push_back(playerLeftClip2);
    player.spriteClips.push_back(playerLeftClip3);
    player.spriteClips.push_back(playerLeftClip4);
    player.spriteClips.push_back(playerRightClip1);
    player.spriteClips.push_back(playerRightClip2);
    player.spriteClips.push_back(playerRightClip3);
    player.spriteClips.push_back(playerRightClip4);

    // Initialize water textures
    water.spriteSheet = new TextureWrap(renderer, waterTexPath);

    // Initialize door textures
    door.spriteSheet = new TextureWrap(renderer, doorTexPath);
    SDL_Rect doorClip = {193, 384, 46, 95};
    door.spriteClips.push_back(doorClip);

    // Initialize platforms
    platforms.spriteSheet = new TextureWrap(renderer, platformsTexPath);

    // Initialize menu sprite sheets
    losemenu.spriteSheet = new TextureWrap(renderer, loseMenuImg);
    pausemenu.spriteSheet = new TextureWrap(renderer, pauseMenuImg);
    startmenu.spriteSheet = new TextureWrap(renderer, startMenuImg);
    winmenu.spriteSheet = new TextureWrap(renderer, winMenuImg);
}

void gameDisplay::update(vector<object> objects, vector<bool> keys, bool grounded) {
	// Update objects
	player.setPos(objects.at(0).getXCoord(), objects.at(0).getYCoord());
    door.setPos(objects.at(1).getXCoord(), objects.at(1).getYCoord());
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
    if (keys[2] && !keys[3]) {
        if (grounded && !(keys[0] || keys[4])) { // Left and on the ground
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
        } else { // left and in the air
            player.spriteSheet->render(player.getXPos(), player.getYPos() - camera.y, &(player.spriteClips.at(1))); //make this the sprite for in the air moving left
        }
    }
    //RIGHT
    else if (keys[3] && !keys[2]) {
          if (grounded && !(keys[0] || keys[4])) {  // Right and on the ground
              // Reset frame information for other directions
              for (auto i : player.frames[0])
                  if (i != 2) i = 0;

              // Render the frame
              player.spriteSheet->render(player.getXPos(), player.getYPos() - camera.y,
                                         &(player.spriteClips.at(5 + (player.frames[0][2] / ANIMATION_DELAY))));

              // Increment frame count for right direction
              player.frames[0][2]++;
              if (player.frames[0][2] / ANIMATION_DELAY >= player.frames[1][2])
                  player.frames[0][2] = 0;
          } else { //Right and in the air
              player.spriteSheet->render(player.getXPos(), player.getYPos() - camera.y, &(player.spriteClips.at(5))); //make this the sprite for in the air moving right
          }
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

void gameDisplay::setMenu(const menuStateType& menu) {
	this->menuState = menu;
}

menuStateType gameDisplay::getMenu() {
	return this->menuState;
}

//returns false if we're leaving the menu system, otherwise true
bool gameDisplay::updateMenu(vector<bool> keys) {
	// Clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
	int arrowx;
	int arrowy;

	if(menuState == start) {
		if(keys[0]) { //up
			if(optionSelected == 0) {
				optionSelected = 1; //goto quit option
			} else if(optionSelected == 1) { //goto start game option
				optionSelected--;
			}
		} else if(keys[1]) { //down
			if(optionSelected == 1) { //goto start game option
				optionSelected = 0;
			} else if(optionSelected == 0) { //goto quit option
				optionSelected++;
			}
		} else if(keys[6]) { //enter
			if(optionSelected == 0) { //start the game
				return false;
			} else if(optionSelected == 1) {
				menuState = quit;
				this->close();
				return true;
			}
		}

		if(optionSelected == 0) { //start game
			arrowx = 340;
			arrowy = 365;
		} else { //quit
			arrowx = 340;
			arrowy = 465;
		}

		//render menu sprite to buffer
		startmenu.spriteSheet->render(0, 0);
		//render arrow to buffer
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, arrowx, arrowy, arrowx + 20, arrowy + 20);
		SDL_RenderDrawLine(renderer, arrowx + 20, arrowy + 20, arrowx, arrowy + 40);

	} else if(menuState == pause) {
		if(keys[0]) { //up
				if(optionSelected == 0) { //goto quit
					optionSelected = 2;
				} else {
					optionSelected--;
				}
		} else if(keys[1]) { //down
			if(optionSelected == 2) { //goto continue
				optionSelected = 0;
			} else {
				optionSelected++;
			}
		} else if(keys[6]) { //space
			 if(optionSelected == 0) { //return to the game
				 return false;
			 } else if(optionSelected == 1) { //return to the menu
				 menuState = start;
				 return true;
			 } else if(optionSelected == 2){ //quit
				 menuState = quit;
				 this->close();
				 return true;
			 }
		}
		if(optionSelected == 0) { //continue
			arrowx = 340;
			arrowy = 310;
		} else if (optionSelected == 1) { //return to main menu
			arrowx = 280;
			arrowy = 410;
		} else { //quit
			arrowx = 340;
			arrowy = 510;
		}

		//render menu sprite to buffer
		pausemenu.spriteSheet->render(0, 0);
		//render arrow to buffer
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, arrowx, arrowy, arrowx + 20, arrowy + 20);
		SDL_RenderDrawLine(renderer, arrowx + 20, arrowy + 20, arrowx, arrowy + 40);
	} else if(menuState == win) {
		if(keys[6]) { //enter
			//menuState = start; //for demo purposes return home
			return false;
		}

		//render menu sprite to buffer
		winmenu.spriteSheet->render(0, 0);
		//render arrow to buffer
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, 280, 380, 300, 400);
		SDL_RenderDrawLine(renderer, 300, 400, 280, 420);
	} else { //lose
		if(keys[6]) { //enter
			menuState = start;
			return true;
		}
		//render menu sprite to buffer
		losemenu.spriteSheet->render(0, 0);
		//render arrow to buffer
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, 280, 380, 300, 400);
		SDL_RenderDrawLine(renderer, 300, 400, 280, 420);
	}

	// Dumping buffer to screen
	SDL_RenderPresent(renderer);

	return true;
}

void gameDisplay::close() {
    // Free textures
    delete player.spriteSheet;
    delete water.spriteSheet;
    delete door.spriteSheet;
    delete platforms.spriteSheet;
    delete losemenu.spriteSheet;
    delete pausemenu.spriteSheet;
    delete startmenu.spriteSheet;
    delete winmenu.spriteSheet;

    player.spriteSheet = nullptr;
    water.spriteSheet = nullptr;
    door.spriteSheet = nullptr;
    platforms.spriteSheet = nullptr;
    losemenu.spriteSheet = nullptr;
    pausemenu.spriteSheet = nullptr;
    startmenu.spriteSheet = nullptr;
    winmenu.spriteSheet = nullptr;

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

#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "physics.h"
#include "sprite.h"
using namespace std;

enum menuStateType { start, pause, win, lose, quit };   // Enumerates the different types of menus

class gameDisplay {
public:
    gameDisplay();                                                                  // Initialize null objects
    gameDisplay(const string& windowName, const int& wdth, const int& hght);        // Constructor
    void update(vector<Object> objects, vector<bool> keys, bool grounded);          // Updates all objects
    void setMenu(const menuStateType& menu);
    menuStateType getMenu();
    bool updateMenu(vector<bool> keys);
    void close();                                                                   // Free up resources
private:
    void initTextures();                                                            // Initializes textures

    // Main window
	SDL_Window* window;
	int WIDTH;
	int HEIGHT;

	SDL_Renderer* renderer; // Main renderer

	// Game sprites
    sprite player;
    sprite water;
	sprite door;
	sprite platforms;

	// Menu sprites
	sprite losemenu;
	sprite pausemenu;
	sprite startmenu;
	sprite winmenu;

    SDL_Rect camera;  // Rectangle to track player movement in the level

    // Menu variables
    menuStateType menuState;
    int optionSelected;
};

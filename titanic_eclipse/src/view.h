/**
 * Header for view classes
 */

#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "physics.h"
#include "sprite.h"
using namespace std;

enum menuStateType { start, pause, win, lose, quit };

class gameDisplay {
public:
    gameDisplay();                                                                  // Initialize null objects
    gameDisplay(const string& windowName, const int& wdth, const int& hght);        // Constructor
    void levelInit(const int& doorX, const int& doorY);                             // Initialize a level
    void update(vector<object> objects, vector<bool> keys, bool grounded, bool win, bool lose);    // Updates all objects
    void close();                                                                   // Free up resources
    void setMenu(const menuStateType& menu);
    menuStateType getMenu();
    bool updateMenu(vector<bool> keys);
private:
    // Main window
	SDL_Window* window;
	int WIDTH;
	int HEIGHT;

	SDL_Renderer* renderer; // Main renderer

	// Sprites
    sprite player;
    sprite water;
	sprite door;
	sprite platforms;
	sprite losemenu;
	sprite pausemenu;
	sprite startmenu;
	sprite winmenu;

    SDL_Rect camera;  // Rectangle to track player movement in the level

    menuStateType menuState;
    int optionSelected;
};

// Handles SDL exceptions
class SDLException: public exception {
public:
    explicit SDLException(string  msg);
    string& what(); // Reports the error message
private:
    string errMsg;
};

class SDLImgException: public exception {
public:
    explicit SDLImgException(string  msg);
    string& what(); // Reports the error message
private:
    string errMsg;
};

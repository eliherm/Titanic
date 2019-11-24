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

class gameDisplay {
public:
    gameDisplay();                                                                  // Initialize null objects
    gameDisplay(const string& windowName, const int& hght, const int& wdth);        // Constructor
    void levelInit(const int& doorX, const int& doorY);                             // Initialize a level
    void update(vector<object> objects, vector<bool> keys, bool win, bool lose);    // Updates all objects
    void close();                                                                   // Free up resources
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

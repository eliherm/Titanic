/**
 * Header for view classes
 */

#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "physics.h"
using namespace std;

class sprite {
public:
	sprite();
	sprite(const int& xpos, const int& ypos, const int& width, const int& height);
	void setPos(const int& xpos, const int& ypos);
	void setDim(const int& width, const int& height);

	int getWidth() const;
	int getHeight() const;
	int getXPos() const;
	int getYPos() const;

private:
	int xcoord;
	int ycoord;
	int width;
	int height;
};

class gameDisplay {
public:
	gameDisplay(const string& windowName, const int& wdth, const int& hght);
	void levelInit(const int& doorX, const int& doorY);
	void update(vector<object> objects, bool win, bool lose);
	void close();

private:
    // Main window
	SDL_Window* window;
	int WIDTH;
	int HEIGHT;

	SDL_Renderer* renderer; // Main renderer

	// Sprites
	sprite door;
	sprite player;
	sprite water;
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

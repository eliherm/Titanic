/**
 * Header for view classes
 */

#pragma once

#include <SDL.h>
#include <vector>
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
	gameDisplay(const int& hght, const int& wdth);
	void levelInit(const int& doorX, const int& doorY);
	void update(const int& playerX, const int& playerY, vector<sprite> platforms, const int& waterY, bool win, bool lose);
	void close();

private:
	SDL_Window* window;
	int WIDTH;
	int HEIGHT;
	SDL_Renderer* renderer;
	sprite door;
	sprite player;
	sprite water;
};

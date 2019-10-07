/**
 * Header for view classes
 */
#pragma once

#include <vector>
#include <SDL.h>

using namespace std;

class sprite {
public:
	sprite();
	sprite(int xpos, int ypos, int width, int height);
	void setPos(int xpos, int ypos);
	void setDim(int width, int height);
	int getWidth();
	int getHeight();
	int getXPos();
	int getYPos();
private:
	int xcoord;
	int ycoord;
	int width;
	int height;
};

class gameDisplay {
public:
	gameDisplay(int hght, int wdth);
	void levelInit(int doorX, int doorY);
	void update(int playerX, int playerY, vector<sprite> platforms, int waterY, bool win, bool lose);
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

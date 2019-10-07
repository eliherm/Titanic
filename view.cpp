#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <stdio.h>

#include "view.h"

using namespace std;

sprite::sprite() {
	this->xcoord = 0;
	this->ycoord = 0;
	this->width = 0;
	this->height = 0;
}

void sprite::setPos(int xpos, int ypos) {
	this->xcoord = xpos;
	this->ycoord = ypos;
}

void sprite::setDim(int width, int height) {
	this->width = width;
	this->height = height;
}

int sprite::getWidth() {
	return this->width;
}

int sprite::getHeight() {
	return this->height;
}

int sprite::getXPos() {
	return this->xcoord;
}

int sprite::getYPos() {
	return this->ycoord;
}

gameDisplay::gameDisplay(int height, int width) {
	//initialize SDL

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	} else {
		HEIGHT = height;
		WIDTH = width;
		window = SDL_CreateWindow("Titanic", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
		if(window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		} else {
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if(renderer == NULL) {
				printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
			} else {
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}
		}
	}
}

void gameDisplay::levelInit(int doorX, int doorY) {
	//setting player, water, and door dimensions
	player.setDim(40, 80);
	water.setDim(WIDTH, 20);
	door.setDim(40, 80);

	//setting door position
	door.setPos(doorX, doorY);
}

void gameDisplay::update(int playerX, int playerY, vector<sprite> platforms, int waterY, bool win, bool lose) {
	//update objects
	player.setPos(playerX, playerY);
	water.setPos(0, waterY);

	//draw background
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	//draw door
	SDL_Rect doorRect = {door.getXPos(), door.getYPos(), door.getWidth(), door.getHeight()};
	SDL_SetRenderDrawColor(renderer, 0xd2, 0xb4, 0x8c, 0xff);
	SDL_RenderFillRect(renderer, &doorRect);

	//draw water
	SDL_Rect waterRect = {0, waterY, water.getWidth(), water.getHeight()};
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xff, 0xff);
	SDL_RenderFillRect(renderer, &waterRect);

	//draw platforms
	SDL_SetRenderDrawColor(renderer, 0xc2, 0xc5, 0xcc, 0xff);
	for(int i = 0; i < platforms.size(); i++) {
		SDL_Rect platform = {platforms.at(i).getXPos(), platforms.at(i).getYPos(), platforms.at(i).getWidth(), platforms.at(i).getHeight()};
		SDL_RenderFillRect(renderer, &platform);
	}

	//draw player
	SDL_Rect playerRect = {player.getXPos(), player.getYPos(), player.getWidth(), player.getHeight()};
	SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
	SDL_RenderFillRect(renderer, &playerRect);

	//dumping buffer to screen
	SDL_RenderPresent(renderer);
}

void gameDisplay::close() {
	//destroying everything!
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;

	//quit SDL subsystems
	SDL_Quit();
}

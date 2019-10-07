#pragma once

#include <SDL2/SDL.h>
#include <string>
using namespace std;

class GameLoop {
public:
    GameLoop(string windowName, const int& width, const int& height);
    void close();
private:
    SDL_Window* mainWindow;

    // Window properties
    string windowName;
    const int width;
    const int height;

    SDL_Renderer* mainRenderer; // Window renderer
    SDL_Texture* mainTexture;

    void init();
    SDL_Texture* loadTexture(const string& path) const;
    void loadMedia();
    void mainLoop();
};

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

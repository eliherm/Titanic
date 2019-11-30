#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>

using namespace std;

class TextureWrap {
public:
    explicit TextureWrap(SDL_Renderer* renderer, const string& path);
    ~TextureWrap();

    // Loads image at specified path
    void loadFromFile(const string& path);

    // Renders texture at given point
    void render(const int& x, const int& y, SDL_Rect* clip = nullptr) const;
private:
    void free();    // De-allocates textures

    SDL_Texture* mainTexture;
    SDL_Renderer* renderer;

    // Image dimensions
    int imgWidth;
    int imgHeight;
};

class Sprite {
public:
    Sprite();
    Sprite(const int& xpos, const int& ypos, const int& width, const int& height);
    void setPos(const int& xpos, const int& ypos);
    void setDim(const int& wdth, const int& hght);
    void enableAnimation(const vector<vector<int>>& framesInfo);

    int getWidth() const;
    int getHeight() const;
    int getXPos() const;
    int getYPos() const;

    TextureWrap* spriteSheet;
    vector<SDL_Rect> spriteClips;
    vector<vector<int>> frames; // Holds frame information for animation
private:
    int xcoord;
    int ycoord;
    int width;
    int height;
};

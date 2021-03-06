#include "sprite.h"
#include "view.h"
#include "sdlException.h"

#ifdef WINDOWS
    #include <SDL2/SDL_image.h>
#else
    #include <SDL_image.h>
#endif

Sprite::Sprite() : xcoord(0), ycoord(0), width(0), height(0), spriteSheet(nullptr) {}
Sprite::Sprite(const int &xpos, const int &ypos, const int &width, const int &height) {
    xcoord = xpos;
    ycoord = ypos;
    this->width = width;
    this->height = height;
    spriteSheet = nullptr;
}

// Set sprite dimensions
void Sprite::setDim(const int& wdth, const int& hght) {
    width = wdth;
    height = hght;
}

// Set sprite position
void Sprite::setPos(const int &xpos, const int &ypos) {
    xcoord = xpos;
    ycoord = ypos;
}

// Initializes a sprite with animation information
void Sprite::enableAnimation(const vector<vector<int>>& framesInfo) {
    frames = vector<vector<int>>(framesInfo);
}

// Sprite Accessors
int Sprite::getWidth() const { return width; }
int Sprite::getHeight() const { return height; }
int Sprite::getXPos() const { return xcoord; }
int Sprite::getYPos() const { return ycoord; }

TextureWrap::TextureWrap(SDL_Renderer* renderer, const string& path) : renderer(renderer), imgWidth(0), imgHeight(0) {
    mainTexture =nullptr;
    loadFromFile(path);
}

TextureWrap::~TextureWrap() {
    free();
}

// Loads an image into the texture
void TextureWrap::loadFromFile(const string& path) {
    // Get rid of preexisting texture
    free();

    // Final texture
    SDL_Texture* newTexture = nullptr;

    // Load image
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        throw SDLImgException("Unable to load image '" + path + "'");
    }

    // Color key image
    //SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));

    // Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (newTexture == nullptr) {
        throw SDLException("Unable to create texture from '" + path + "'");
    }

    // Get image dimensions
    imgWidth = loadedSurface->w;
    imgHeight = loadedSurface->h;

    // Free up loaded surface
    SDL_FreeSurface(loadedSurface);

    //set all changes to main texture
    mainTexture = newTexture;
}

void TextureWrap::free() { //getting rid of a texture
    // Free texture if it exists
    if (mainTexture != nullptr) {
        SDL_DestroyTexture(mainTexture);
        mainTexture = nullptr;
    }
}

void TextureWrap::render(const int& x, const int& y, SDL_Rect* clipRec) const {
    // Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, imgWidth, imgHeight };

    // Set clip rendering dimensions
    if (clipRec != nullptr) {
        renderQuad.w = clipRec->w;
        renderQuad.h = clipRec->h;
    }

    SDL_RenderCopy(renderer, mainTexture, clipRec, &renderQuad);
}

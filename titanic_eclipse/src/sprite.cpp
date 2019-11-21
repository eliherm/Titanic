#include <SDL_image.h>
#include "sprite.h"
#include "view.h"

sprite::sprite() {
    this->xcoord = 0;
    this->ycoord = 0;
    this->width = 0;
    this->height = 0;
    this->spriteSheet = nullptr;
}

sprite::sprite(const int &xpos, const int &ypos, const int &width, const int &height) {
    this->xcoord = xpos;
    this->ycoord = ypos;
    this->width = width;
    this->height = height;
    this->spriteSheet = nullptr;
}

void sprite::setDim(const int& wdth, const int& hght) {
    this->width = wdth;
    this->height = hght;
}

void sprite::setPos(const int &xpos, const int &ypos) {
    xcoord = xpos;
    ycoord = ypos;
}

// Sprite Accessors
int sprite::getWidth() const { return this->width; }
int sprite::getHeight() const { return this->height; }
int sprite::getXPos() const { return this->xcoord; }
int sprite::getYPos() const { return this->ycoord; }


TextureWrap::TextureWrap(SDL_Renderer* renderer, const string& path) : renderer(renderer), imgWidth(0), imgHeight(0) {
    mainTexture =nullptr;
    loadFromFile(path);
}

TextureWrap::~TextureWrap() {
    free();
}

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
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));

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

    mainTexture = newTexture;
}

void TextureWrap::free() {
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
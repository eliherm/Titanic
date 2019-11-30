#include <SDL2/SDL.h>

#ifdef WINDOWS
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_mixer.h>
#else
    #include <SDL_image.h>
    #include <SDL_mixer.h>
#endif

#include "sdlException.h"

SDLException::SDLException(string msg): errMsg(move(msg)) {}
string& SDLException::what() {
    errMsg += "\nSDL Error: " + static_cast<string>(SDL_GetError());
    return errMsg;
}

SDLImgException::SDLImgException(string msg): errMsg(move(msg)) {}
string& SDLImgException::what() {
    errMsg += "\nSDL_image Error: " + static_cast<string>(IMG_GetError());
    return errMsg;
}

SDLMixException::SDLMixException(string msg): errMsg(move(msg)) {}
string& SDLMixException::what() {
    errMsg += "\nSDL_mixer Error: " + static_cast<string>(Mix_GetError());
    return errMsg;
}

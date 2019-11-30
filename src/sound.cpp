#include <iostream>
#include <SDL2/SDL.h>
#include "sdlException.h"
#include "sound.h"
using namespace std;

Sound::Sound() {
    try {
        // Initialize SDL Audio
        if (SDL_Init(SDL_INIT_AUDIO) < 0) {
            throw SDLException("SDL could not initialize!");
        }

        // Initialize SDL Mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
            throw SDLMixException("SDL_mixer could not initialize!");
        }

#ifdef WINDOWS
        bgMusicPath = "..\\assets\\theme.wav";
#else
        bgMusicPath = "../assets/theme.wav";
#endif

        // Load the background music
        bgMusic = Mix_LoadMUS(bgMusicPath.c_str());
        if (bgMusic == nullptr) {
            throw SDLMixException("Unable to load file '" + bgMusicPath + "'");
        }
    } catch (SDLMixException& e) {
        cerr << e.what() << endl;
        exit(1);
    } catch(SDLException& e) {
        cerr << e.what() << endl;
        exit(1);
    }
}

void Sound::play() {
    // Play the background music
    if (Mix_PlayMusic(bgMusic, -1) == -1) {
        throw SDLMixException("The track '" + bgMusicPath + "' could not be played");
    }
}

void Sound::close() {
    // Free the track
    Mix_FreeMusic(bgMusic);
    bgMusic = nullptr;

    // Quit SDL Mix
    Mix_Quit();
}

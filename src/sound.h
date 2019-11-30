#pragma once

#ifdef WINDOWS
    #include <SDL2/SDL_mixer.h>
#else
    #include <SDL_mixer.h>
#endif

class Sound {
public:
    Sound();
    void play();
    void close();
private:
    string bgMusicPath;
    Mix_Music* bgMusic;
};

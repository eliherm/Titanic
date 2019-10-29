#pragma once

#include <SDL2/SDL.h>
#include <string>
using namespace std;

class keyboardInput {
public:
    explicit keyboardInput(const SDL_Event& e);
    vector<int> getKeyboardInput();

private:
    SDL_Event event;
};

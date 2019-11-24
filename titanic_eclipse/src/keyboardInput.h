#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>

using namespace std;

class keyboardInput {
public:
    explicit keyboardInput();
    vector<bool> getKeyboardInput();

private:					// {0,   1,    2     3       4      5   }
    vector<bool> keyStates; // {up, down, left, right, space, escape}
};

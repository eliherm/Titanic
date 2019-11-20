#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>

using namespace std;

class keyboardInput {
public:
    explicit keyboardInput();
    vector<bool> getKeyboardInput();

private:
    vector<bool> keyStates; // Index 0: Up, Index 1: down, index 2: left, index 3: right, index 4: space, index 5: escape
};

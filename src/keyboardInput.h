#pragma once

#include <vector>
using namespace std;

class KeyboardInput {
public:
    explicit KeyboardInput();
    vector<bool> getKeyboardInput();

private:
    /*
     * Index 0 -> Up
     * Index 1 -> Down
     * Index 2 -> Left
     * Index 3 -> Right
     * Index 4 -> Escape
     * Index 5 -> Return
     */
    vector<bool> keyStates;
};

#include "keyboardInput.h"

keyboardInput::keyboardInput() {
    for (int i = 0; i < 5; i++) {
        keyStates.push_back(false);
    }
}

vector<bool> keyboardInput::getKeyboardInput() {
    // Check if a key was pressed
    const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
    if (currentKeyStates[ SDL_SCANCODE_UP ] || currentKeyStates[ SDL_SCANCODE_W ]) {
        keyStates[0] = true;
    } else {
        keyStates[0] = false;
    }

    if (currentKeyStates[ SDL_SCANCODE_DOWN ] || currentKeyStates[ SDL_SCANCODE_S ]) {
        keyStates[1] = true;
    } else {
        keyStates[1] = false;
    }

    if (currentKeyStates[ SDL_SCANCODE_LEFT ] || currentKeyStates[ SDL_SCANCODE_A ]) {
        keyStates[2] = true;
    } else {
        keyStates[2] = false;
    }

    if (currentKeyStates[ SDL_SCANCODE_RIGHT ] || currentKeyStates[ SDL_SCANCODE_D ]) {
        keyStates[3] = true;
    } else {
        keyStates[3] = false;
    }

    if (currentKeyStates[ SDL_SCANCODE_SPACE ]) {
        keyStates[4] = true;
    } else {
        keyStates[4] = false;
    }

    return keyStates;
}

#include "keyboardInput.h"

keyboardInput::keyboardInput() { //on start, the keys will be initialized to false, but will never be forced back to false to avoid rapid switching
    for (int i = 0; i < 7; i++) {
        keyStates.push_back(false);
    }
}

vector<bool> keyboardInput::getKeyboardInput() { //called by controller, to check current key states
    const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr); // Check if a key was pressed

    //UP
    if (currentKeyStates[ SDL_SCANCODE_UP ] || currentKeyStates[ SDL_SCANCODE_W ]) { //if either of the relevant keys for the direction are pressed
        keyStates[0] = true; //set the subsequent index to true
    } else {
        keyStates[0] = false; //set the subsequent index to false
    }

    //DOWN
    if (currentKeyStates[ SDL_SCANCODE_DOWN ] || currentKeyStates[ SDL_SCANCODE_S ]) {
        keyStates[1] = true;
    } else {
        keyStates[1] = false;
    }

    //LEFT
    if (currentKeyStates[ SDL_SCANCODE_LEFT ] || currentKeyStates[ SDL_SCANCODE_A ]) {
        keyStates[2] = true;
    } else {
        keyStates[2] = false;
    }

    //RIGHT
    if (currentKeyStates[ SDL_SCANCODE_RIGHT ] || currentKeyStates[ SDL_SCANCODE_D ]) {
        keyStates[3] = true;
    } else {
        keyStates[3] = false;
    }

    //SPACE
    if (currentKeyStates[ SDL_SCANCODE_SPACE ]) {
        keyStates[4] = true;
    } else {
        keyStates[4] = false;
    }

    //ESCAPE
    if (currentKeyStates[ SDL_SCANCODE_ESCAPE ]) {
        keyStates[5] = true;
    } else {
        keyStates[5] = false;
    }

    if (currentKeyStates[ SDL_SCANCODE_ESCAPE ]) {
          keyStates[5] = true;
      } else {
          keyStates[5] = false;
      }

    if (currentKeyStates[ SDL_SCANCODE_RETURN ]) {
          keyStates[6] = true;
      } else {
          keyStates[6] = false;
      }

    return keyStates; //return the vector with the updated boolean values
}

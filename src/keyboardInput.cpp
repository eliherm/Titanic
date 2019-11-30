#include <SDL2/SDL.h>
#include <iostream>
#include "keyboardInput.h"

KeyboardInput::KeyboardInput() {
    // Initialize key states to false
    for (int i = 0; i < 7; i++) {
        keyStates.push_back(false);
    }
}

vector<bool> KeyboardInput::getKeyboardInput() {
    const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr); // Check if a key was pressed

    // Up
    keyStates.at(0) = (currentKeyStates[ SDL_SCANCODE_UP ] || currentKeyStates[ SDL_SCANCODE_W ] ||
            currentKeyStates[ SDL_SCANCODE_SPACE ]);

    // Down
    keyStates.at(1) = (currentKeyStates[ SDL_SCANCODE_DOWN ] || currentKeyStates[ SDL_SCANCODE_S ]);

    // Left
    keyStates.at(2) = (currentKeyStates[ SDL_SCANCODE_LEFT ] || currentKeyStates[ SDL_SCANCODE_A ]);

    // Right
    keyStates.at(3) = (currentKeyStates[ SDL_SCANCODE_RIGHT ] || currentKeyStates[ SDL_SCANCODE_D ]);

    // Escape
    keyStates.at(4) = currentKeyStates[ SDL_SCANCODE_ESCAPE ];

    // Return
    keyStates.at(5) = currentKeyStates[ SDL_SCANCODE_RETURN ];

    return keyStates;
}

#include "keyboardInput.h"

keyboardInput::keyboardInput(const SDL_Event& e) : event(e) {}

// TODO: Process WASD keys
vector<int> keyboardInput::getKeyboardInput() {
    vector<int> keyCodes;

    // Check if a key was pressed
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                keyCodes.push_back(SDLK_UP);
                break;

            case SDLK_DOWN:
                keyCodes.push_back(SDLK_DOWN);
                break;

            case SDLK_LEFT:
                keyCodes.push_back(SDLK_LEFT);
                break;

            case SDLK_RIGHT:
                keyCodes.push_back(SDLK_RIGHT);
                break;

            case SDLK_w:
            	keyCodes.push_back(SDLK_UP);
            	break;

            case SDLK_a:
                 keyCodes.push_back(SDLK_LEFT);
                 break;

            case SDLK_s:
                 keyCodes.push_back(SDLK_DOWN);
                 break;

            case SDLK_d:
            	 keyCodes.push_back(SDLK_RIGHT);
                 break;

            case SDLK_SPACE:
                 keyCodes.push_back(SDLK_SPACE);
                 break;
        }
    }

    return keyCodes;
}

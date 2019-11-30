#include <iostream>
#include <SDL2/SDL.h>
#include <time.h>
#include <vector>
#include "controller.h"
#include "sdlException.h"

#ifdef WINDOWS
    #include <SDL2/SDL_mixer.h>
#else
    #include <SDL_mixer.h>
#endif

const int WIDTH = 960;
const int HEIGHT = 720;

int main() {
    Controller c(60, 60);
    c.run();
    c.music.close();
    c.activeScreen.close();
    return 0;
}

Controller::Controller(int fps, int tps) {
	f_time = CLOCKS_PER_SEC / fps;
	t_time = CLOCKS_PER_SEC / tps;
	running = false;
	inmenu = true;
	level = 1;

	try {
        activeScreen = gameDisplay("titanic", WIDTH, HEIGHT);	// Initialize the game window
        activeScreen.setMenu(start);							// Begin with the start menu
        keyboardIo = KeyboardInput();
        music.play();
    } catch (SDLImgException& e) {
        cerr << e.what() << endl;
        exit(1);
    } catch (SDLMixException& e) {
        cerr << e.what() << endl;
        exit(1);
    } catch(SDLException& e) {
        cerr << e.what() << endl;
        exit(1);
    }
}

void Controller::run() {
	int prev_time = clock();
	int new_time = 0;
	int f = 0;
	int t = 0;
	int tcnt = 0;

	running = true;
	while (running) {
	    SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if(e.type == SDL_QUIT) {
                running =  false;
            }
        }

		new_time = clock();
		if (new_time > prev_time) {
			f += new_time - prev_time;
			t += new_time - prev_time;
		}

		prev_time = new_time;
		if (f >= f_time) {
			f -= f_time;
			doFrame();
		}

		if (t >= t_time) {
			t -= t_time;
			tcnt += 1;

			if (!inmenu) {										    // when gameplay is occuring
				if (activeEngine.getWinState()) {				    // win condition (reach door)
					activeScreen.setMenu(win);					    // set win menu to active menu
					inmenu = true;								    // switch to menu view
				} else if (activeEngine.getLoseState()) {			// lose condition (hit water)
					activeScreen.setMenu(lose);					    // set lose menu to active menu
					inmenu = true;								    // switch to menu view
				} else if(getKeyStates().at(4)) {					// click escape (our acting pause button)
					activeScreen.setMenu(pause);				    // set pause menu to active menu
					inmenu = true;								    // switch to menu view
				} else {
					doPhysics();								    // regular gameplay occuring
				}
			} else if (tcnt > 4) {
				if (!activeScreen.updateMenu(getKeyStates()) && !(activeScreen.getMenu() == quit)) {
					inmenu = false;
					if (activeScreen.getMenu() == start) {
						// load level 1
						level = 1;  // reset level for new game

                        // load the level from a text file
						activeEngine = PhysicsEngine("level" + to_string(level) + ".txt");
					} else if(activeScreen.getMenu() == win) {
						if (level == 5) {
						    // Final level reached
						    // Reset to start menu
							activeScreen.setMenu(start);
							inmenu = true;
						} else {
						    // Load the next level
							level++;
							activeEngine = PhysicsEngine("level" + to_string(level) + ".txt");
						}
					}
				} else if(activeScreen.getMenu() == quit) {
					running = false;
				}
				tcnt = 0;
			}
		}
	}
}

/*
 * Takes the width and height of the window and returns an array of all objects centered around the player within
 * that window.
 * */
vector<Object> Controller::getVisibleObjects(const int& width, const int& height) const {
	vector<Object> all = activeEngine.getState();
	vector<Object> visible;
	double playerX = all.at(0).getXCoord() + all.at(0).getWidth() / static_cast<double>(2);
    double playerY = all.at(0).getYCoord() + all.at(0).getHeight() / static_cast<double>(2);

	for (const auto &temp : all) {
        // checks each object against the borders of the screen.
        // if we notice latency issues and fps > tps, it might be worth moving this into getState
        if ((temp.getXCoord() <= playerX + WIDTH / static_cast<double>(2)
            || temp.getXCoord() + temp.getWidth() >= playerX - WIDTH / static_cast<double>(2))
            && (temp.getYCoord() <= playerY + HEIGHT / static_cast<double>(2)
            || temp.getYCoord() + temp.getHeight() >= playerY - HEIGHT / static_cast<double>(2))) {
                visible.push_back(temp);
        }
	}

	return visible;
}

vector<bool> Controller::getKeyStates() {
    // TODO: Check for escape keys outside of regular gameplay
	vector<bool> keyStates = keyboardIo.getKeyboardInput();
	return keyStates;
}

void Controller::doPhysics() {
	activeEngine.updateObjects(getKeyStates());
}

void Controller::doFrame() {
	if (!inmenu) {
		activeScreen.update(getVisibleObjects(WIDTH, HEIGHT), getKeyStates(), activeEngine.getState().at(0).isGrounded());
	}
}

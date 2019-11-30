#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "keyboardInput.h"
#include "physics.h"
#include "sprite.h"
#include "view.h"

using namespace std;

int main(int argc, char *argv[]);

class Controller {
public:
	Controller(int fps, int tps);
	void run();							//now also contains all menu information
	vector<object> getVisibleObjects(const int&, const int&);
	vector<bool> getKeyStates();
	bool running = false;

    physicsEngine activeEngine;
    gameDisplay activeScreen;
    keyboardInput keyboardIo;
private:
	int f_time;
	int t_time;
	int level = 1;
	bool inmenu;

	void doPhysics();
	void doFrame();
};

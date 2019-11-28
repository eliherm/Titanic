#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "keyboardInput.h"
#include "physics.h"
using namespace std;

int main(int argc, char *argv[]);

class Controller {
public:
	Controller(int fps, int tps);
	void run();
	vector<object> getVisibleObjects(const int&, const int&);
	vector<bool> getKeyStates();
	void getGraphicData();
	bool running = false;

    physicsEngine activeEngine; // will not always be the only reference to an engine, simply to denote which engine is action in the event we have two at once
    gameDisplay activeScreen;
    keyboardInput keyboardIo;
private:
	int f_time;
	int t_time;
	bool inmenu;

	void doPhysics();
	void doFrame();
};

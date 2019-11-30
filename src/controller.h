#pragma once

#include <vector>
#include "keyboardInput.h"
#include "physics.h"
#include "sound.h"
#include "sprite.h"
#include "view.h"
using namespace std;

class Controller {
public:
	Controller(int fps, int tps);
	void run();
	vector<Object> getVisibleObjects(const int&, const int&) const;
	vector<bool> getKeyStates();

    PhysicsEngine activeEngine;
    gameDisplay activeScreen;
    KeyboardInput keyboardIo;
    Sound music;

    bool running;
private:
	int f_time;     // Frame time
	int t_time;     // Tick time
	int level;
	bool inmenu;

	void doPhysics();
	void doFrame();
};
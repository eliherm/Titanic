#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "physics.h"
using namespace std;

int main(int argc, char *argv[]);

physicsEngine activeEngine;//should never be the only reference to an engine, simply to denote which engine is action in the event we have two at once

class Controller {
public:
	Controller(int fps, int tps);
	void run();
	vector<object> getVisibleObjects(const int, const int);
	void getKeyStates();
	void getGraphicData();
	bool running;
private:
	int f_time;
	int t_time;

	void doPhysics();
	void doFrame();
	void checkKeys();
};

#pragma once

#include <SDL2/SDL.h>
#include <string>
using namespace std;

int main(int argc, char *argv[]);

class Controller {
public:
	Controller(int fps, int tps);
	void run();
	void getVisibleObjects();
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

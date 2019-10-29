#include <SDL2/SDL.h>
#include <time.h>

#include "controller.h"

int main(int argc, char *argv[]) {

}

Controller::Controller(int fps, int tps) {
	this->f_time = CLOCKS_PER_SEC / fps;
	this->t_time = CLOCKS_PER_SEC / tps;
}

void Controller::run() {
	int prev_time = clock();
	int new_time = 0;
	int f = 0;
	int t = 0;

	running = true;
	while(running){//running is a public variable, so can be switched to false whenever needed
		checkKeys();//some keys (such as whatever we choose to be quit) may operate outside of regular frames or ticks, so it will be checked every loop
		new_time = clock();
		if(new_time > prev_time){//as far as I understand clock can wrap around, I'm not sure what the max value is yet. this code will miss a loop whenever it wraps, I might fix it at some point but the impact is minor
			f += new_time - prev_time;
			t += new_time - prev_time;
		}
		prev_time = new_time;
		if(f >= f_time){
			f -= f_time;//if we run into issues or I have extra time we can use modulo here, but will need to have a system to track lost ticks and pass that info along to be dealt with properly
			doFrame();
		}
		if(t >= t_time){
			t -= t_time;//if we run into issues or I have extra time we can use modulo here, but will need to have a system to track lost ticks and pass that info along to be dealt with properly
			doPhysics();
		}
	}
}

void Controller::getVisibleObjects() {//will not be void, this is simply a template to be adjusted as soon as a final decision is made on variable formatting
}

void Controller::getKeyStates() {//will not be void, this is simply a template to be adjusted as soon as a final decision is made on variable formatting
}

void Controller::getGraphicData() {//not needed at the moment, but here for easy implementation if it's needed later
}

void Controller::doPhysics() {//to be implemented in the physics branch
}

void Controller::doFrame() {//to be implemented in the view branch
}

void Controller::checkKeys() {//to be implemented in the keys branch
}

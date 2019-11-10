#include <SDL2/SDL.h>
#include <time.h>

#include "controller.h"

int main(int argc, char *argv[]) {

	//main menu, initialization of objects, etc

	Controller c(60, 60);
	c.run();
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

//untested!! it's relatively simple so its unlikely to have errors, but should be tested once view and physics are on-board
vector<object> Controller::getVisibleObjects(const int width, const int height) {//takes width and height of the window, returns an array of all objects within that window, centered on the player. with the player as the first object. in the future object should include an id of some sort that can be translated to a sprite
	vector<object> all = activeEngine.getState();
	vector<object> visible;
	int playerX = all.at(0).getXCoord() + all.at(0).getWidth()/2;
	int playerY = all.at(0).getYCoord() + all.at(0).getHeight()/2;
	for(int i = 0; i < all.size(); i++){
		object temp = all.at(i);//checks each object against the borders of the screen. if we notice latency issues and fps > tps, it might be worth moving this into getState
		if((temp.getXCoord() <= playerX + width/2 || temp.getXCoord() + temp.getWidth() >= playerX - width/2)
				&& (temp.getYCoord() <= playerY + height/2 || temp.getYCoord() + temp.height >= playerY - height/2)){
			visible.push_back(temp);
		}
	}
	return visible;
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

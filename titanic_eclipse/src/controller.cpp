#include <iostream>
#include <time.h>
#include <SDL2/SDL.h>
#include <vector>
#include "view.h"
#include "physics.h"
#include "keyboardInput.h"
#include "controller.h"

const int WIDTH = 960;
const int HEIGHT = 720;

int main(int argc, char *argv[]) {
	//main menu, initialization of objects, etc
	Controller c(60, 60);
	c.run();
	c.activeScreen.close();
	return 0;
}

Controller::Controller(int fps, int tps) {
	this->f_time = CLOCKS_PER_SEC / fps;
	this->t_time = CLOCKS_PER_SEC / tps;

	this->inmenu = true;

	try {
        activeScreen = gameDisplay("titanic", WIDTH, HEIGHT);
        activeScreen.setMenu(start);
        keyboardIo = keyboardInput();
    } catch (SDLImgException& e) {
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
	while (running) {//running is a public variable, so can be switched to false whenever needed

	    SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if(e.type == SDL_QUIT) {
                running =  false;
            }
        }

		//checkKeys();//some keys (such as whatever we choose to be quit) may operate outside of regular frames or ticks, so it will be checked every loop
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
			tcnt += 1;

			if(!inmenu) {
				if(activeEngine.getWinState()) {
					activeScreen.setMenu(win);
					inmenu = true;
				} else if(activeEngine.getLoseState()){
					activeScreen.setMenu(lose);
					inmenu = true;
				} else if(getKeyStates()[5]) {
					activeScreen.setMenu(pause);
					inmenu = true;
				} else {
					doPhysics();
				}
			} else if(tcnt > 4){
				if(!activeScreen.updateMenu(getKeyStates()) && !(activeScreen.getMenu() == quit)) {
					inmenu = false;

					if(activeScreen.getMenu() == start) {
						//load level 1
						object player = object(300, 400, 128, 240, 0.5);
						object door = object(900, 110, 40, 80, 0);
						object water = object(0, 700, 960, 690, 0);
						vector<object> platforms;
						platforms = vector<object>();
                        platforms.push_back(object(214, 650, 300, 10, 0));
                        platforms.push_back(object(550, 500, 300, 10, 0));
                        platforms.push_back(object(100, 330, 300, 10, 0));
                        platforms.push_back(object(700, 200, 300, 10, 0));

						activeEngine = physicsEngine(player, door, water, platforms);
					}
				} else if(activeScreen.getMenu() == quit) {
					running = false;
				}
				tcnt = 0;
			}
		}
	}
}

vector<object> Controller::getVisibleObjects(const int& width, const int& height) {//takes width and height of the window, returns an array of all objects within that window, centered on the player. with the player as the first object. in the future object should include an id of some sort that can be translated to a sprite
	vector<object> all = activeEngine.getState();
	vector<object> visible;
	int playerX = all.at(0).getXCoord() + all.at(0).getWidth()/2;
	int playerY = all.at(0).getYCoord() + all.at(0).getHeight()/2;
	for(int i = 0; i < all.size(); i++){
		object temp = all.at(i);//checks each object against the borders of the screen. if we notice latency issues and fps > tps, it might be worth moving this into getState
		if((temp.getXCoord() <= playerX + width/2 || temp.getXCoord() + temp.getWidth() >= playerX - width/2)
				&& (temp.getYCoord() <= playerY + height/2 || temp.getYCoord() + temp.getHeight() >= playerY - height/2)){
			visible.push_back(temp);
		}
	}
	return visible;
}

vector<bool> Controller::getKeyStates() {//will not be void, this is simply a template to be adjusted as soon as a final decision is made on variable formatting
	vector<bool> keyStates = keyboardIo.getKeyboardInput();
	//check for escape keys that operate outside of normal gameplay, such as escape for pause menu (not yet implemented)
	return keyStates;
}

void Controller::getGraphicData() {//not needed at the moment, but here for easy implementation if it's needed later
}

void Controller::doPhysics() {//to be implemented in the physics branch
	activeEngine.updateObjects(getKeyStates());
}

void Controller::doFrame() {//to be implemented in the view branch
	if(!inmenu) {
		activeScreen.update(getVisibleObjects(WIDTH, HEIGHT), getKeyStates(), activeEngine.getState()[0].isGrounded());
	}/* else {
		if(!activeScreen.updateMenu(getKeyStates())) {
			inmenu = false;
		}
	}*/
}

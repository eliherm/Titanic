#include <iostream>
#include <time.h>
#include <SDL2/SDL.h>
#include <vector>
#include "view.h"
#include "physics.h"
#include "keyboardInput.h"
#include "controller.h"

#ifdef WINDOWS
    #include <SDL2/SDL_mixer.h>
#else
    #include <SDL_mixer.h>
#endif

const int WIDTH = 960;
const int HEIGHT = 720;

int main(int argc, char *argv[]) {
	//load theme
	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Music *gMusic = NULL;
#ifdef WINDOWS
	gMusic = Mix_LoadMUS("..\\titanic\\titanic_eclipse\\assets\\theme.wav");
#else
	gMusic = Mix_LoadMUS("../titanic_eclipse/assets/theme.wav");
#endif
	if(Mix_PlayMusic(gMusic, -1) == -1) { //starts music with troubleshooting if music doesnt work
		printf("Mix_PlayMusic: %s\n", Mix_GetError());
	}

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
        activeScreen = gameDisplay("titanic", WIDTH, HEIGHT);	//generate the window
        activeScreen.setMenu(start);							//starts off the game on the start menu
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

			if(!inmenu) {										//when gameplay is occuring
				if(activeEngine.getWinState()) {				//win condition(reach door)
					activeScreen.setMenu(win);					//set win menu to active menu
					inmenu = true;								//switch to menu view
				} else if(activeEngine.getLoseState()){			//lose condition (hit water)
					activeScreen.setMenu(lose);					//set lose menu to active menu
					inmenu = true;								//switch to menu view
				} else if(getKeyStates()[5]) {					//click escape (our acting pause button
					activeScreen.setMenu(pause);				//set pause menu to active menu
					inmenu = true;								//switch to menu view
				} else {
					doPhysics();								//regular gameplay occuring, check whats going on
				}
			} else if(tcnt > 4){								//for navigating on menu screen
				if(!activeScreen.updateMenu(getKeyStates()) && !(activeScreen.getMenu() == quit)) {
					inmenu = false;

					if(activeScreen.getMenu() == start) {
						//load level 1
						level = 1;															//reset level for new game
						activeEngine = physicsEngine("level" + to_string(level) + ".txt"); //for loading in levels from a text file
					}else if(activeScreen.getMenu() == win){
						if(level == 5){														//if youve reached our current final level
							activeScreen.setMenu(start);									//go back to start menu
							inmenu = true;
						}else{
							level++;														//beaten the level, so load up the next one
							activeEngine = physicsEngine("level" + to_string(level) + ".txt");
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
	}
}

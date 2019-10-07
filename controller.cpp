
#include <vector>
#include "view.h"

int main() {
	gameDisplay gd(720, 960);

	gd.levelInit(40, 40);

	vector<sprite> platforms;
	sprite platform1;
	platform1.setPos(20, 120);
	platform1.setDim(175, 20);
	platforms.push_back(platform1);
	sprite platform2;
	platform2.setPos(370, 380);
	platform2.setDim(175, 20);
	platforms.push_back(platform2);

	bool quit = false;

	//Event handler
	SDL_Event e;

	//while application is running
	while(!quit) {
		//Event loop: handles events on queue
		while(SDL_PollEvent(&e) != 0) {
			//User requests quit
			if(e.type == SDL_QUIT) {
				quit = true;
			}

			gd.update(450, 300, platforms, 700, false, false);
		}
	}

	gd.close();

	return 0;
}

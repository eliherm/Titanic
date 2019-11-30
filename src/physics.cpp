#include <cmath>
#include <iostream>
#include <fstream>
#include "physics.h"

const double PLAYER_GRAVITY = 0.5, JUMP_SPEED = 8, MAX_RUN_SPEED = 7, RUN_ACCELERATION = 0.6;
const int FALL_THRESHOLD = 30;  // Duration a platform is stable (FALL_THRESHOLD / 60 -> seconds)
using namespace std;

Object::Object(): xcoord(0), ycoord(0), xspeed(0), yspeed(0), width(0), height(0), gravity(0), grounded(true),
    platformfallctr(0) {}

Object::Object(const Object& newval) {
	xcoord = newval.xcoord;
	ycoord = newval.ycoord;
	xspeed = newval.xspeed;
	yspeed = newval.yspeed;
	width = newval.width;
	height = newval.height;
	gravity = newval.gravity;
	grounded = newval.grounded;
	platformfallctr = newval.platformfallctr;
}

Object::Object(const double& xpos, const double& ypos, const int& width, const int& height, const double& gravity) {
	xcoord = xpos;
	ycoord = ypos;
	xspeed = 0;
	yspeed = 0;
	this->width = width;
	this->height = height;
	this->gravity = gravity;
	grounded = true;
	platformfallctr = 0;
}

void Object::setCoord(const double& newx, const double& newy) {
    xcoord = newx;
    ycoord = newy;
}

void Object::addCoord(const double& newx, const double& newy) {
    xcoord += newx;
    ycoord += newy;
}

double Object::getXCoord() const { return xcoord; }
double Object::getYCoord() const { return ycoord; }

void Object::setWidth(const int& newval) { width = newval; }
void Object::setHeight(const int& newval) { height = newval; }
int Object::getWidth() const { return width; }
int Object::getHeight() const { return height; }

void Object::setXSpeed(const double& newval) { xspeed = newval; }
void Object::addXSpeed(const double& newval) { xspeed += newval; }
double Object::getXSpeed() const { return xspeed; }

void Object::setYSpeed(const double& newval) { yspeed = newval; }
void Object::addYSpeed(const double& newval) { yspeed += newval; }
double Object::getYSpeed() const { return yspeed; }

void Object::setGrav(const double& newval) { gravity = newval; }
double Object::getGrav() const { return gravity; }

void Object::setGrounded(const bool& newval) { grounded = newval; }
bool Object::isGrounded() const { return grounded; }

void Object::setPFC(const int& pfc) { platformfallctr = pfc; }
int Object::getPFC() const { return platformfallctr; }

PhysicsEngine::PhysicsEngine(): win(false), lose(false) {}
PhysicsEngine::PhysicsEngine(const Object& player, const Object& door, const Object& water, const vector<Object>& platforms) {
	this->player = player;
	this->door = door;
	this->water = water;
	this->platforms = platforms;

	win = false;
	lose = false;
}

PhysicsEngine::PhysicsEngine(const string& level) {
#ifdef WINDOWS
    string prefix = "..\\assets\\levels\\";
#else
    string prefix = "../assets/levels/";
#endif

	ifstream fileIn(prefix + level);
	if (fileIn.fail()) {
        cerr << "Error: '" << level << "' could not be loaded" << endl;
        exit(1);
    }

    string x, y, width, height;
    getline(fileIn, x, '\t');
    getline(fileIn, y, '\t');
    getline(fileIn, width, '\t');
    getline(fileIn, height);
    player = Object(stod(x), stod(y), stoi(width), stoi(height), PLAYER_GRAVITY);

    getline(fileIn, x, '\t');
    getline(fileIn, y, '\t');
    getline(fileIn, width, '\t');
    getline(fileIn, height);
    door = Object(stod(x), stod(y), stoi(width), stoi(height), 0);

    getline(fileIn, x, '\t');
    getline(fileIn, y, '\t');
    getline(fileIn, width, '\t');
    getline(fileIn, height);
    water = Object(stod(x), stod(y), stoi(width), stoi(height), 0);

    platforms = vector<Object>();
    while (getline(fileIn, x, '\t')) {
        getline(fileIn, y, '\t');
        getline(fileIn, width, '\t');
        getline(fileIn, height);
        platforms.emplace_back(Object(stod(x), stod(y), stoi(width), stoi(height), 0));
    }

    fileIn.close();

    win = false;
    lose = false;
}

void PhysicsEngine::updateObjects(const vector<bool>& keypresses) {
	if (keypresses.at(0)) {
	    // Up direction
		if (player.isGrounded()) {
			player.setGrounded(false);
			player.setYSpeed(-JUMP_SPEED);  // Initial bump in upward velocity
		} else {
			// Maintain jump. Allows for better height if held
			if (player.getYSpeed() <= -(JUMP_SPEED / 10)) {
				player.addYSpeed(-PLAYER_GRAVITY / 2);
			}
		}
	}

	if (keypresses.at(1)) {
	    // Down direction
		player.addYSpeed(PLAYER_GRAVITY);
	}

	if (keypresses.at(2) && !keypresses.at(3)) {
	    // Left direction
		if (player.getXSpeed() > RUN_ACCELERATION - MAX_RUN_SPEED) {
			player.addXSpeed(-RUN_ACCELERATION);
		} else {
			player.setXSpeed(-MAX_RUN_SPEED);
		}
	} else if (keypresses.at(3) && !keypresses.at(2)) {
        // Right direction
		if (player.getXSpeed() < MAX_RUN_SPEED - RUN_ACCELERATION) {
			player.addXSpeed(RUN_ACCELERATION);
		} else {
			player.setXSpeed(MAX_RUN_SPEED);
		}
	} else {
		if (player.getXSpeed() > RUN_ACCELERATION) {
			player.addXSpeed(-RUN_ACCELERATION);
		} else if (player.getXSpeed() < -RUN_ACCELERATION) {
			player.addXSpeed(RUN_ACCELERATION);
		} else
		    player.setXSpeed(0);
	}

	// Apply gravity to relevant objects (currently only player)
	player.addYSpeed(PLAYER_GRAVITY);

	// Check collisions between relevant objects (currently only player) and all objects.
	// If others are implemented, will be placed in a loop iterating through the list
	auto movement = new double[2];
	movement[0] = player.getXSpeed();
	movement[1] = player.getYSpeed();

	for (auto &platform : platforms) {
        double* temp = getMaxVector(player, platform);
        if (abs(temp[0]) < abs(movement[0])) {
            // Wall hit, x vector is closer to 0
            movement[0] = temp[0];
            player.setXSpeed(0);
        }

        if (temp[1] < movement[1] && movement[1] > 0) {
            // Floor hit
            movement[1] = temp[1];
            player.setYSpeed(0);
            player.setGrounded(true);   // Only ground on floor hit, not ceiling hit

            /*
            platform.setPFC(platform.getPFC() + 1);
            if (platform.getPFC() > FALL_THRESHOLD)
                platform.setYSpeed(platform.getYSpeed() + 0.2);
            */
        } else if (temp[1] > movement[1] && movement[1] < 0) {
            // Ceiling hit
            movement[1] = temp[1];
            player.setYSpeed(0);
        }

        platform.addCoord(0, platform.getYSpeed());
        delete[] temp;
	}

	// If no collisions, move object (before checking the next object), else compute collision behavior

	if ((player.getXCoord() + movement[0] >= 0) && (player.getXCoord() + movement[0] <= 960 - player.getWidth())) {
        player.addCoord(movement[0], movement[1]);
	} else {
        player.addCoord(0, movement[1]);
	}

	delete[] movement;

	// Move water up screen
	water.setCoord(water.getXCoord(), water.getYCoord() - 1);

	// Check door and water for win/loss
	if (checkIntersection(player, door)) {
        win = true; //Win Condition
	} else if (checkIntersection(player, water)) {
        lose = true; //Lose Condition
	}
}

bool PhysicsEngine::checkIntersection(const Object& obj1, const Object& obj2) const {
	for (int i = 0; i < obj1.getWidth(); ++i) {
		double x = obj1.getXCoord() + i;
        double topy = obj1.getYCoord();
        double bottomy = obj1.getYCoord() + obj1.getHeight();

		if ((x >= obj2.getXCoord() && x <= obj2.getXCoord() + obj2.getWidth())
				&& ((bottomy >= obj2.getYCoord() && bottomy <= obj2.getYCoord() + obj2.getHeight())
						|| (topy >= obj2.getYCoord() && topy <= obj2.getYCoord() + obj2.getHeight()))) {
			return true;
		}
	}

	for (int i = 0; i < obj1.getHeight(); ++i) {
        double leftx = obj1.getXCoord();
        double rightx = obj1.getXCoord() + obj1.getWidth();
        double y = obj1.getYCoord() + i;

		if((y >= obj2.getYCoord() && y <= obj2.getYCoord() + obj2.getHeight())
				&& ((leftx >= obj2.getXCoord() && leftx <= obj2.getXCoord() + obj2.getWidth())
						|| (rightx >= obj2.getXCoord() && rightx <= obj2.getXCoord() + obj2.getWidth()))) {
			return true;
		}
	}
	return false;
}

double* PhysicsEngine::getMaxVector(const Object& obj1, const Object& obj2) const {
	auto result = new double[2];
	double speed = sqrt(pow(obj1.getXSpeed(), 2) + pow(obj1.getYSpeed(), 2));
	double xnorm = obj1.getXSpeed()/speed;
	double ynorm = obj1.getYSpeed()/speed;

	Object obj1copy(obj1);
	int segments = 0;
	while (!checkIntersection(obj1copy, obj2) && !checkIntersection(obj2, obj1copy) && segments < speed) {
		obj1copy.setCoord(obj1copy.getXCoord() + xnorm, obj1copy.getYCoord() + ynorm);
		segments++;
	}

	// If it doesn't collide
	if (!checkIntersection(obj1copy, obj2)) {
		result[0] = obj1.getXSpeed();
		result[1] = obj1.getYSpeed();
		return result;
	}

	// correct for bullet paper problem
	// If(segments > 0) { segments should always be greater than 0, otherwise obj1 is already inside obj2

    // Check which face was hit, and only reduce that part of the vector
	obj1copy.setCoord(obj1copy.getXCoord() - xnorm, obj1copy.getYCoord());
	if (checkIntersection(obj1copy, obj2)) {
		result[0] = obj1.getXSpeed();
		result[1] = obj1copy.getYCoord() - obj1.getYCoord() - ynorm;
	} else {
		result[0] = obj1copy.getXCoord() - obj1.getXCoord();
		result[1] = obj1.getYSpeed();
	}

	return result;
}

vector<Object> PhysicsEngine::getState() const {
	vector<Object> tempVec {};
	tempVec.push_back (player);
	tempVec.push_back (door);
	tempVec.push_back (water);

	for (auto const &platform : platforms) {
	    tempVec.push_back(platform);
	}

	return tempVec;
}

bool PhysicsEngine::getWinState() const { return win; }
bool PhysicsEngine::getLoseState() const { return lose; }

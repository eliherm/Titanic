//#define WINDOWS

/**
 * Source file for physics engine
 */
#include <cmath>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "physics.h"

#ifdef WINDOWS
    string prefix = "..\\titanic\\titanic_eclipse\\levels\\";
#else
    string prefix = "../titanic_eclipse/levels/";
#endif
const double PLAYERGRAVITY = 0.5, JUMPSPEED = 12, MAXRUNSPEED = 7, RUNACCELERATION = 0.6; //Originally: 0.5, 10, 2, 0.2
const int FALLTHRESHOLD = 30; //platforms remains stable for FALLTHRESHOLD/60 seconds

using namespace std;

object::object() {
	xcoord = 0;
	ycoord = 0;
	//speed = 0;
	xspeed = 0;
	yspeed = 0;
	width = 0;
	height = 0;
	gravity = 0;
}

object::object(const object& newval) {
	xcoord = newval.xcoord;
	ycoord = newval.ycoord;
	//speed = 0;
	xspeed = newval.xspeed;
	yspeed = newval.yspeed;
	width = newval.width;
	height = newval.height;
	gravity = newval.gravity;
	grounded = newval.grounded;
	platformfallctr = newval.platformfallctr;
}

object::object(const double& xpos, const double& ypos, const int& width, const int& height, const double& gravity) {
	xcoord = xpos;
	ycoord = ypos;
	xspeed = 0;
	yspeed = 0;
	this->width = width;
	this->height = height;
	this->gravity = gravity;
}

double object::getXCoord() { return xcoord; }
double object::getYCoord() { return ycoord; }

void object::setCoord(const double& newx, const double& newy) {
	xcoord = newx;
	ycoord = newy;
}

void object::addCoord(const double& newx, const double& newy) {
	xcoord += newx;
	ycoord += newy;
}

int object::getWidth() {
	return width;
}

void object::setWidth(const int& newval) {
	width = newval;
}

int object::getHeight() {
	return height;
}

void object::setHeight(const int& newval) {
	height = newval;
}

double object::getXSpeed() {
	return this->xspeed;
}

void object::setXSpeed(const double& newval) {
	this->xspeed = newval;
}

void object::addXSpeed(const double& newval) {
	this->xspeed += newval;
}

double object::getYSpeed() {
	return this->yspeed;
}

void object::setYSpeed(const double& newval) {
	this->yspeed = newval;
}

void object::addYSpeed(const double& newval) {
	this->yspeed += newval;
}

double object::getGrav() {
	return gravity;
}

void object::setGrav(const double& newval) {
	gravity = newval;
}

bool object::isGrounded() {
	return grounded;
}

void object::setGrounded(const bool& newval) {
	grounded = newval;
}

int object::getPFC() {
	return this->platformfallctr;
}

void object::setPFC(const int& pfc) {
	this->platformfallctr = pfc;
}

physicsEngine::physicsEngine(): win(false), lose(false) {}
physicsEngine::physicsEngine(const object& player, const object& door, const object& water, const vector<object>& platforms) {
	this->player = player;
	this->door = door;
	this->water = water;
	this->platforms = platforms;

	win = false;
	lose = false;
}

physicsEngine::physicsEngine(const string level) {
	ifstream fileIn(prefix + level);
	if (fileIn.fail()) cout << "fail" << endl;
		string x, y, width, height;
		getline(fileIn, x, '\t');
		getline(fileIn, y, '\t');
		getline(fileIn, width, '\t');
		getline(fileIn, height);
		player = object(stod(x), stod(y), stod(width), stod(height), PLAYERGRAVITY);
		getline(fileIn, x, '\t');
		getline(fileIn, y, '\t');
		getline(fileIn, width, '\t');
		getline(fileIn, height);
		door = object(stod(x), stod(y), stod(width), stod(height), 0);
		getline(fileIn, x, '\t');
		getline(fileIn, y, '\t');
		getline(fileIn, width, '\t');
		getline(fileIn, height);
		water = object(stod(x), stod(y), stod(width), stod(height), 0);
		platforms = vector<object>();
		while (getline(fileIn, x, '\t')){
			getline(fileIn, y, '\t');
			getline(fileIn, width, '\t');
			getline(fileIn, height);
			platforms.push_back(object(stod(x), stod(y), stod(width), stod(height), 0));
		}

		win = false;
		lose = false;
}

void physicsEngine::updateObjects(const vector<bool> &keypresses) {
	if(keypresses[0]){//up
		if(player.isGrounded()){//simply checks if on the ground for basic jumps. may eventually include collision checks with wall etc for other functionality
			player.setGrounded(false); //change to the state of the character
			player.setYSpeed(-JUMPSPEED);//jump, initial bump in upward velocity
		} else {
			//maintain jump. will allow better height if held, will probably not apply if the player is already moving down, or nearing the top of the jump, as this makes things feel floaty
			if(player.getYSpeed() <= -(JUMPSPEED/10)){
				player.addYSpeed(-PLAYERGRAVITY/2);
			}
		}
	}
	if(keypresses[4]){//space, separate from up in case we implement ladders or such
		if(player.isGrounded()){//simply checks if on the ground for basic jumps. may eventually include collision checks with wall etc for other functionality
			player.setGrounded(false); //change to the state of the character
			player.setYSpeed(-JUMPSPEED);//jump, initial bump in upward velocity
		}else{
			//maintain jump. will allow better height if held, will probably not apply if the player is already moving down, or nearing the top of the jump, as this makes things feel floaty
			if(player.getYSpeed() <= -(JUMPSPEED/10)){
				player.addYSpeed(-PLAYERGRAVITY/2);
			}
		}
	}
	if(keypresses[1]){//down
		player.addYSpeed(PLAYERGRAVITY);//fastfall
	}
	if(keypresses[2] && !keypresses[3]){//left
		if(player.getXSpeed() > RUNACCELERATION - MAXRUNSPEED){
			player.addXSpeed(-RUNACCELERATION);
		}else{
			player.setXSpeed(-MAXRUNSPEED);
		}
	}else if(keypresses[3] && !keypresses[2]){//right
		if(player.getXSpeed() < MAXRUNSPEED - RUNACCELERATION){
			player.addXSpeed(RUNACCELERATION);
		}else{
			player.setXSpeed(MAXRUNSPEED);
		}
	}else{
		if(player.getXSpeed() > RUNACCELERATION){
			player.addXSpeed(-RUNACCELERATION);
		}else if(player.getXSpeed() < -RUNACCELERATION){
			player.addXSpeed(RUNACCELERATION);
		}else player.setXSpeed(0);
	}

	//apply gravity to relevant objects (currently only player)
	player.addYSpeed(PLAYERGRAVITY);

	//check collisions between relevant objects (currently only player) and all objects. if others are implemented, will be placed in a loop iterating through the list
	double* movement = new double[2];
	movement[0] = player.getXSpeed();
	movement[1] = player.getYSpeed();
	for(int i = 0; i < platforms.size(); i++){
		double* temp = getMaxVector(player, platforms.at(i));
		if(abs(temp[0]) < abs(movement[0])){//hit a wall, x vector is closer to 0
			movement[0] = temp[0];
            player.setXSpeed(0);
		}
		if(temp[1] < movement[1] && movement[1] > 0){//hit a floor
			movement[1] = temp[1];
            player.setYSpeed(0);
			player.setGrounded(true);//only ground on floor hit, not ceiling hit
			platforms.at(i).setPFC(platforms.at(i).getPFC() + 1);
			if(platforms.at(i).getPFC() > FALLTHRESHOLD) {
				platforms.at(i).setYSpeed(platforms.at(i).getYSpeed() + 0.3);
			}
		}else if(temp[1] > movement[1] && movement[1] < 0){//hit a ceiling
			movement[1] = temp[1];
			player.setYSpeed(0);
		}

		platforms.at(i).addCoord(0, platforms.at(i).getYSpeed());

		delete[] temp;
	}

	//if no collisions, move object (before checking the next object), else compute collision behavior
    player.addCoord(movement[0], movement[1]);
	delete[] movement;

	//move water up screen
	water.setCoord(water.getXCoord(), water.getYCoord() - 1);

	//check door and water for win/loss
	if(checkIntersection(player, door)){
        this->win = true;   //Win Condition
	}else if(checkIntersection(player, water)){
        this->lose = true;	//Lose Condition
	}
}

bool physicsEngine::checkIntersection(object obj1, object obj2) {
	for(int i = 0; i < obj1.getWidth(); ++i) {
		int x = obj1.getXCoord() + i;
		int topy = obj1.getYCoord();
		int bottomy = obj1.getYCoord() + obj1.getHeight();

		if((x >= obj2.getXCoord() && x <= obj2.getXCoord() + obj2.getWidth())
				&& ((bottomy >= obj2.getYCoord() && bottomy <= obj2.getYCoord() + obj2.getHeight())
						|| (topy >= obj2.getYCoord() && topy <= obj2.getYCoord() + obj2.getHeight()))) {
			return true;
		}
	}

	for(int i = 0; i < obj1.getHeight(); ++i) {
		int leftx = obj1.getXCoord();
		int rightx = obj1.getXCoord() + obj1.getWidth();
		int y = obj1.getYCoord() + i;

		if((y >= obj2.getYCoord() && y <= obj2.getYCoord() + obj2.getHeight())
				&& ((leftx >= obj2.getXCoord() && leftx <= obj2.getXCoord() + obj2.getWidth())
						|| (rightx >= obj2.getXCoord() && rightx <= obj2.getXCoord() + obj2.getWidth()))) {
			return true;
		}
	}
	return false;
}

double* physicsEngine::getMaxVector(object obj1, object obj2) {
	double* result = new double[2];
	double speed = sqrt(pow(obj1.getXSpeed(), 2) + pow(obj1.getYSpeed(), 2));
	double xnorm = obj1.getXSpeed()/speed;
	double ynorm = obj1.getYSpeed()/speed;

	object obj1copy(obj1);
	int segments = 0;
	while(!checkIntersection(obj1copy, obj2) && !checkIntersection(obj2, obj1copy) && segments < speed) {
		obj1copy.setCoord(obj1copy.getXCoord() + xnorm, obj1copy.getYCoord() + ynorm);
		segments++;
	}

	//if it doesn't collide
	if(!checkIntersection(obj1copy, obj2)){
		result[0] = obj1.getXSpeed();
		result[1] = obj1.getYSpeed();
		return result;
	}

	//correct for bullet paper problem
	//if(segments > 0) { segments should always be greater than 0, otherwise obj1 is already inside obj2

	obj1copy.setCoord(obj1copy.getXCoord() - xnorm, obj1copy.getYCoord());//check which face was hit, and only reduce that part of the vector
	if(checkIntersection(obj1copy, obj2)){
		result[0] = obj1.getXSpeed();
		result[1] = obj1copy.getYCoord() - obj1.getYCoord() - ynorm;
	}else{
		result[0] = obj1copy.getXCoord() - obj1.getXCoord();
		result[1] = obj1.getYSpeed();
	}

	return result;
}

vector<object> physicsEngine::getState() {
	vector<object> tempVec {};

	// add player -> door -> water
	tempVec.push_back (player); 	//index 0 in vector
	tempVec.push_back (door);		//index 1 in vector
	tempVec.push_back (water);		//index 2 in vector

	// fill tempVec with lots of data - iterate through platform vector and add to temp vec
	for(int i=0; i < platforms.size(); i++){
		tempVec.push_back (platforms[i]); //index 2+i in vector
	}

	return tempVec;
}

bool physicsEngine::getWinState() {
	return this->win;
}

bool physicsEngine::getLoseState() {
	return this->lose;
}

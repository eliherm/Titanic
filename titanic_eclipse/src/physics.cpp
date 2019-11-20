/**
 * Source file for physics engine
 */
#include <vector>
#include <cmath>
#include <stdio.h>
#include "physics.h"

using namespace std;

object::object() {
	xcoord = 0;
	ycoord = 0;
	speed = 0;
	width = 0;
	height = 0;
	gravity = 0;
}

object::object(const double& xpos, const double& ypos, const int& width, const int& height, const double& gravity) {
	xcoord = xpos;
	ycoord = ypos;
	speed = 0;
	this->width = width;
	this->height = height;
	this->gravity = gravity;
}

double object::getXCoord() {
	return xcoord;
}

double object::getYCoord() {
	return ycoord;
}

void object::setCoord(const double& newx, const double& newy) {
	xcoord = newx;
	ycoord = newy;
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

double object::getSpeed() {
	return speed;
}

void object::setSpeed(const double& newval) {
	speed = newval;
}

double object::getDir() {
	return direction;
}

void object::setDir(const double& newval) {
	direction = newval;
}

double object::getGrav() {
	return gravity;
}

void object::setGrav(const double& newval) {
	gravity = newval;
}

physicsEngine::physicsEngine(object player, object door, object water, vector<object> platforms) {
	this->player = player;
	this->door = door;
	this->water = water;
	this->platforms = platforms;
}

bool physicsEngine::checkIntersection(object obj1, object obj2) {
	//
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
	double xnorm = cos(obj1.getDir());
	double ynorm = -sin(obj1.getDir()); //reflect y movement to match reflected SDL coordinate system

	object obj1copy = obj1;

	int segments = 0;
	while(!checkIntersection(obj1copy, obj2) && !checkIntersection(obj2, obj1copy) && segments < obj1.getSpeed()) {
		obj1copy.setCoord(obj1copy.getXCoord() + xnorm, obj1copy.getYCoord() + ynorm);
		segments++;
	}

	//correct for bullet paper problem
	if(segments > 0) {
		obj1copy.setCoord(obj1copy.getXCoord() - xnorm, obj1copy.getYCoord() - ynorm);
	}

	result[0] = floor(obj1copy.getXCoord() - obj1.getXCoord());
	result[1] = -floor(obj1copy.getYCoord() - obj1.getYCoord()); //reflecting y movement back

	return result;
}

vector<object> physicsEngine::getState() {

    vector<object> tempVec {};

	// add player -> door -> water
	tempVec.push_back (player);
	tempVec.push_back (door);
	tempVec.push_back (water);

	// fill tempVec with lots of data - iterate through platform vector and add to temp vec
	for(int i=0; i < platforms.size(); i++){
   		tempVec.push_back (platforms[i]);
	}


    return tempVec;

}

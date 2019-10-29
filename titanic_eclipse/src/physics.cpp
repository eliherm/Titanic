/**
 * Source file for physics engine
 */
#include <vector>
#include "physics.h"

object::object(const int& xpos, const int& ypos, const float& gravity) {
	xcoord = xpos;
	ycoord = ypos;
	this->gravity = gravity;
}

int object::getXCoord() {
	return xcoord;
}

int object::getYCoord() {
	return ycoord;
}

int object::setCoord(const int& newx, const int& newy) {
	xcoord = newx;
	ycoord = newy;
}

float object::getSpeed() {
	return speed;
}

void object::setSpeed(const float& newval) {
	speed = newval;
}

float object::getDir() {
	return direction;
}

void object::setDir(const float& newval) {
	direction = newval;
}

float object::getGrav() {
	return gravity;
}

void object::setGrav(const float& newval) {
	gravity = newval;
}

void physicsEngine::initLevel(object player, object door, object water, vector<object> platforms) {
	this->player = player;
	this->door = door;
	this->water = water;
	this->platforms = platforms;
}

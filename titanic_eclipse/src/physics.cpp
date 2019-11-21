/**
 * Source file for physics engine
 */
#include <cmath>
#include <stdio.h>
#include <vector>
#include "physics.h"

const int PLAYERGRAVITY = 5;

using namespace std;

object::object() {
	xcoord = 0;
	ycoord = 0;
	speed = 0;
	width = 0;
	height = 0;
	gravity = 0;
}

object::object(const int& xpos, const int& ypos, const int& width, const int& height, const double& gravity) {
	xcoord = xpos;
	ycoord = ypos;
	speed = 0;
	this->width = width;
	this->height = height;
	this->gravity = gravity;
}

int object::getXCoord() {
    return xcoord;
}

int object::getYCoord() {
    return ycoord;
}

void object::setCoord(const int& newx, const int& newy) {
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

bool object::isGrounded() {
    return grounded;
}

void object::setGrounded(const bool newval) {
    grounded = newval;
}

physicsEngine::physicsEngine() {
	player = object(416, 300, 128, 240, PLAYERGRAVITY);
	door = object(400, 50, 40, 80, 0);
	water = object(0, 600, 960, 10, 0);
	platforms = vector<object>();
	platforms.emplace_back(object(330, 540, 300, 10, 0));
    platforms.emplace_back(object(770, 300, 200, 10, 0));
    platforms.emplace_back(object(0, 150, 200, 10, 0));
    platforms.emplace_back(object(400, 50, 300, 10, 0));
}

physicsEngine::physicsEngine(object player, object door, object water, vector<object> platforms) {
	this->player = player;
	this->door = door;
	this->water = water;
	this->platforms = platforms;
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

double* physicsEngine::lineIntersect(double vec1x, double vec1y, int p1x, int p1y, double vec2x, double vec2y, int p2x, int p2y) {
	double* result = new double[2];

	double ddet =  (vec1x*vec2y) - (vec1y*vec2x);
	if(ddet == 0) {
		return nullptr;
	} else {
		double t2 = ((vec1x * (p2y - p1y)) - (vec1y * (p2x - p1x)))/ddet;
		result[0] = p2x + (t2 * vec2x);
		result[1] = p2y + (t2 * vec2y);
		return result;
	}
}

void physicsEngine::updateObjects(const vector<bool> &keypresses) {
	if(keypresses[0]){//up
			if(player.isGrounded()){//simply checks if on the ground for basic jumps. may eventually include collision checks with wall etc for other functionality
				player.setGrounded(false);
				//jump, initial bump in upward velocity
			}else{
				//maintain jump. will allow better height if held, will probably not apply if the player is already moving down, or nearing the top of the jump, as this makes things feel floaty
			}
	}
	if(keypresses[4]){//space, separate from up in case we implement ladders or such
			if(player.isGrounded()){//simply checks if on the ground for basic jumps. may eventually include collision checks with wall etc for other functionality
				player.setGrounded(false);
				//jump, initial bump in upward velocity
			}else{
				//maintain jump. will allow better height if held, will probably not apply if the player is already moving down, or nearing the top of the jump, as this makes things feel floaty
			}
		}
	if(keypresses[1]){//down
		//fast-fall
	}
	if(keypresses[2] && !keypresses[3]){//left
		//movement, adjust speed and direction
	}else if(keypresses[3] && !keypresses[2]){//right
		//movement, adjust speed and direction
	}else{
		//slow down horizontal movement when not inputting a single direction, allows for precise controls without feeling abrupt
	}

	//apply gravity to relevant objects (currently only player)

	//check collisions between relevant objects (currently only player) and all objects.
	//if no collisions, move object (before checking the next object), else compute collision behavior
}

void physicsEngine::checkEdge(double** res, object obj1, object obj2, int edge) {
	double rad = obj1.getDir();
	double speed = obj1.getSpeed();
	int edgelen = 0;
	int Px = 0;
	int Py = 0;

	if(edge == 0 || edge == 2) {
		edgelen = obj1.getWidth();
	} else {
		edgelen = obj1.getHeight();
	}

	for(int i = 0; i < edgelen; ++i) {
		double** intersections;
		intersections = new double*[4];
		double olddist;
		if(edge == 0) {
			Px = obj1.getXCoord() + i;
			Py = obj1.getYCoord();
		} else if(edge == 1) {
			Px = obj1.getXCoord() + obj1.getWidth();
			Py = obj1.getYCoord() + i;
		} else if(edge == 2) {
			Px = obj1.getXCoord() + i;
			Py = obj1.getYCoord() + obj1.getHeight();
		} else {
			Px = obj1.getXCoord();
			Py = obj1.getYCoord() + i;
		}

		//check against top edge of target
		intersections[0] = lineIntersect(speed * cos(rad), speed * sin(rad), Px, Py, 1.0, 0.0, obj2.getXCoord(), obj2.getYCoord());

		//check against right edge of target
		intersections[1] = lineIntersect(speed * cos(rad), speed * sin(rad), Px, Py, 0.0, -1.0, obj2.getXCoord() + obj2.getWidth(), obj2.getYCoord());

		//check againt bottom edge of target
		intersections[2] = lineIntersect(speed * cos(rad), speed * sin(rad), Px, Py, 1.0, 0.0, obj2.getXCoord(), obj2.getYCoord() + obj2.getHeight());

		//check againt left edge of target
		intersections[3] = lineIntersect(speed * cos(rad), speed * sin(rad), Px, Py, 0.0, -1.0, obj2.getXCoord(), obj2.getYCoord());

		for(int i = 0; i < 3; ++i) {
			if(intersections[i] != nullptr) {
				double resdist = sqrt(pow(intersections[i][0] - (double)Px, 2) + pow(intersections[i][1] - (double)Py, 2));
				if(res[i] != nullptr) {
					if(resdist < olddist && resdist <= speed) {
						res[i] = intersections[i];
						olddist = resdist;
					}
				} else {
					if(resdist <= speed) {
						res[i] = intersections[i];
						olddist = resdist;
					}
				}
			}
		}

		delete intersections;
	}
}

//ironclad collision checker returns points at which an object will pass through another object
//when movement vector is resolved
//returns 2d array of intersections, first intersection at top edge of obj2, moving clockwise (top -> right -> bottom -> left)
//if no intersection occurs against an obj2 edge, that index will be nullptr
double** physicsEngine::checkCollision(object obj1, object obj2) {
	double** result;
	result = new double*[4];
	for(int i = 0; i < 4; ++i) {
		result[i] = new double[2];
		result[i] = nullptr;
	}
	//convert object velocity vector to line at object edge
	double rad = obj1.getDir();
	double speed = obj1.getSpeed();
	printf("vector: [%f, %f]\n", speed * cos(rad), speed * sin(rad));
	if(cos(rad) > 0 && sin(rad) <= 0) { //check right and down
		checkEdge(result, obj1, obj2, 1);
		checkEdge(result, obj1, obj2, 2);
	} else if(cos(rad) <= 0 && sin(rad) < 0) { //check left and down
		checkEdge(result, obj1, obj2, 3);
		checkEdge(result, obj1, obj2, 2);
	} else if(cos(rad) < 0 && sin(rad) >= 0){ //check left and up
		checkEdge(result, obj1, obj2, 3);
		checkEdge(result, obj1, obj2, 0);
	} else { //check right and up
		checkEdge(result, obj1, obj2, 1);
		checkEdge(result, obj1, obj2, 0);
	}

	return result;
}

/*int main(int argc, char *argv[]) {//for quick testing
	object o1(0, 0, 10, 10, 0.0);
	o1.setSpeed(10000);
	o1.setDir(6.2);
	object o2(0, 20, 10, 10000, 0);
	vector<object> v1 = {o2};

	physicsEngine pe(o1, o1, o1, v1);
	pe.checkCollision(o1, o2);
}*/

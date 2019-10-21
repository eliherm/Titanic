/**
 * Header class for physics engine
 */
#pragma once

#include <vector>

class object {
public:
	object(const int& xpos, const int& ypos, const float& gravity);
	int getXCoord();
	int getYCoord();
	void setCoord(const int& newx, const &int newy);
	float getSpeed();
	void setSpeed(const float& newval);
	float getDir();
	void setDir(const float& newval);
	float getGrav();
	void setGrav(const float& newval);
private:
	int xcoord;
	int ycoord;
	float speed; //speed in pixels/s
	float direction; //direction of movement in radii
	float gravity; //acceleration due to gravity in pixels/s^2
};

class physicsEngine {
public:
	void initLevel(object player, object door, object water, vector<object> platforms);
	//vector<vector<object>> getCollisions();
	//void updateObjects();
	//vector<object> getState();
private:
	object player;
	object door;
	object water;
	vector<object> platforms;
};

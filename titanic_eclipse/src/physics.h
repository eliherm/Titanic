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
    physicsEngine(object player, object door, object water, vector<object> platforms);
    void updateObjects(const vector<int>& keypresses); //needs to take whatever the data format for key press input is
    vector<object> getState();
private:
    float** checkCollision(object obj1, object obj2);
    object player;
    object door;
    object water;
    vector<object> platforms;
};

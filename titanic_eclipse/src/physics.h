/**
 * Header class for physics engine
 */
#pragma once

#include <vector>
using namespace std;

class object {
public:
    object();
    object(const double& xpos, const double& ypos, const int& width, const int& height, const double& gravity);
    double getXCoord();
    double getYCoord();
    void setCoord(const double& newx, const double& newy);
    int getWidth();
    int getHeight();
    void setWidth(const int& newval);
    void setHeight(const int& newval);
    double getSpeed();
    void setSpeed(const double& newval);
    double getDir();
    void setDir(const double& newval);
    double getGrav();
    void setGrav(const double& newval);
    bool containsPoint(const double& x, const double& y);
private:
    double xcoord;
    double ycoord;
    int width;
    int height;
    double speed; //speed in pixels/s
    double direction; //direction of movement in radii
    double gravity; //acceleration due to gravity in pixels/s^2
};

class physicsEngine {
public:
    physicsEngine(object player, object door, object water, vector<object> platforms);
    void updateObjects(const vector<int>& keypresses); //needs to take whatever the data format for key press input is
    vector<object> getState();
    bool checkIntersection(object obj1, object obj2);
    double* getMaxVector(object obj1, object obj2);
private:
    object player;
    object door;
    object water;
    vector<object> platforms;
};

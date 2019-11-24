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
    void addCoord(const double& newx, const double& newy);
    int getWidth();
    int getHeight();
    void setWidth(const int& newval);
    void setHeight(const int& newval);
    double getXSpeed();
    void setXSpeed(const double& newval);
    void addXSpeed(const double& newval);
    double getYSpeed();
    void setYSpeed(const double& newval);
    void addYSpeed(const double& newval);
    double getGrav();
    void setGrav(const double& newval);
    bool containsPoint(const double& x, const double& y);
    bool isGrounded();
    void setGrounded(const bool newval);
private:
    bool grounded = true;
    double xcoord;
    double ycoord;
    int width;
    int height;
    double xspeed;
    double yspeed;
    //double speed; //speed in pixels/s
    //double direction; //direction of movement in radii
    double gravity; //acceleration due to gravity in pixels/s^2
};

class physicsEngine {
public:
	physicsEngine();
    physicsEngine(object player, object door, object water, vector<object> platforms);
    void updateObjects(const vector<bool> &keypresses); //needs to take whatever the data format for key press input is
    vector<object> getState();
    bool checkIntersection(object obj1, object obj2);
    double* getMaxVector(object obj1, object obj2);
private:
    object player;
    object door;
    object water;
    vector<object> platforms;
};

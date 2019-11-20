/**
 * Header class for physics engine
 */
#pragma once
 
#include <vector>
using namespace std;
 
class object {
public:
    object();
    object(const int& xpos, const int& ypos, const int& width, const int& height, const double& gravity);
    int getXCoord();
    int getYCoord();
    void setCoord(const int& newx, const int& newy);
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
    bool isGrounded();
    void setGrounded(const bool);//to be implemented via collision
private:
    bool grounded;
    int xcoord;
    int ycoord;
    int width;
    int height;
    double speed; //speed in pixels/s
    double direction; //direction of movement in radii
    double gravity; //acceleration due to gravity in pixels/s^2
};
 
class physicsEngine {
public:
    physicsEngine();
    physicsEngine(object player, object door, object water, vector<object> platforms);
    void updateObjects(const vector<bool>& keypresses); //needs to take whatever the data format for key press input is
    vector<object> getState();
private:
    double* lineIntersect(double vec1x, double vec1y, int p1x, int p1y, double vec2x, double vec2y, int p2x, int p2y);
    double** checkCollision(object obj1, object obj2);
    void checkEdge(double** res, object obj1, object obj2, int edge);
    object player;
    object door;
    object water;
    vector<object> platforms;
};

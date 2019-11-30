#pragma once

#include <vector>
using namespace std;

class Object {
public:
    Object();
    Object(const double& xpos, const double& ypos, const int& width, const int& height, const double& gravity);
    Object(const Object&);

    void setCoord(const double& newx, const double& newy);
    void addCoord(const double& newx, const double& newy);
    double getXCoord() const;
    double getYCoord() const;

    void setWidth(const int& newval);
    void setHeight(const int& newval);
    int getWidth() const;
    int getHeight() const;

    void setXSpeed(const double& newval);
    void addXSpeed(const double& newval);
    double getXSpeed() const;

    void setYSpeed(const double& newval);
    void addYSpeed(const double& newval);
    double getYSpeed() const;

    void setGrav(const double& newval);
    double getGrav() const;

    void setGrounded(const bool& newval);
    bool isGrounded() const;

    void setPFC(const int& pfc);
    int getPFC() const;
private:
    double xcoord;
    double ycoord;
    int width;
    int height;
    double xspeed;
    double yspeed;
    bool grounded;
    int platformfallctr;
    double gravity; // Acceleration due to gravity in pixels / s^2
};

class PhysicsEngine {
public:
	PhysicsEngine();
    PhysicsEngine(const Object& player, const Object& door, const Object& water,const vector<Object>& platforms);
	explicit PhysicsEngine(const string& level);

    void updateObjects(const vector<bool>& keypresses);
    vector<Object> getState() const;
    bool getWinState() const;
    bool getLoseState() const;
private:
    bool checkIntersection(const Object& obj1, const Object& obj2) const;
    double* getMaxVector(const Object& obj1, const Object& obj2) const;

    bool win;
    bool lose;

    Object player;
    Object door;
    Object water;
    vector<Object> platforms;
};

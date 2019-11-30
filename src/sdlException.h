#pragma once

#include <string>
using namespace std;

// Handles SDL exceptions
class SDLException: public exception {
public:
    explicit SDLException(string msg);
    string& what(); // Reports the error message
private:
    string errMsg;
};

class SDLImgException: public exception {
public:
    explicit SDLImgException(string msg);
    string& what(); // Reports the error message
private:
    string errMsg;
};

class SDLMixException: public exception {
public:
    explicit SDLMixException(string msg);
    string& what(); // Reports the error message
private:
    string errMsg;
};


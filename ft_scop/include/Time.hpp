#ifndef TIME_HPP
#define TIME_HPP

#include "glfw-3.4/include/GLFW/glfw3.h"
#include <string>
#include <iostream>
#include <iomanip>

class Time
{
private:
    float lastTime = 0.0f;

public:
    static float deltaTime;
    static float currentTime;

    Time();
    ~Time();

    void update();
};

class Timer
{
private:
    float startTime;
    std::string name;

public:

    Timer();
    ~Timer();

    void start(const std::string& timerName);
    void stop();

};
#endif
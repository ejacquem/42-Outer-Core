#ifndef TIME_HPP
#define TIME_HPP

#include "glfw-3.4/include/GLFW/glfw3.h"

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

#endif
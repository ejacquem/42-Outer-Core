#include "../include/Time.hpp"

float Time::deltaTime = 0.0f;
float Time::currentTime = 0.0f;

Time::Time() {}

Time::~Time() {}

void Time::update()
{
    lastTime = currentTime;
    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
}

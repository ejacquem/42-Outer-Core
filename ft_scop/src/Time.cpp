#include "Time.hpp"

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


Timer::Timer(){}
Timer::~Timer(){}

void Timer::start(const std::string& timerName)
{
    name = timerName;
    startTime = glfwGetTime();
}

void Timer::stop()
{
    std::cout << "Timer " << name << ": " << glfwGetTime() - startTime << " seconds" << std::endl;
}

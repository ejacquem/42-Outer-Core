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
    double elapsedTime = (glfwGetTime() * 1000) - (startTime * 1000);
    std::cout << "Timer " << name << ": " << std::fixed << std::setprecision(6) << elapsedTime << " ms" << std::endl;
}

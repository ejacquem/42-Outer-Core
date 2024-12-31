#include <GLFW/glfw3.h>
#include "Time.h"

Time::Time() {}

Time::~Time() {}

void Time::update()
{
    lastTime = currentTime;
    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
}

#include <GLFW/glfw3.h>

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

float Time::deltaTime = 0.0f;
float Time::currentTime = 0.0f;

#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <GLFW/glfw3.h>
#include "Camera.hpp"
#include "Object.hpp"
#include "Scop.hpp"

#define GET_KEY(key) glfwGetKey(window, GLFW_KEY_##key)
#define GET_KEY_PRESS(key) glfwGetKey(window, GLFW_KEY_##key) == GLFW_PRESS
#define GET_KEY_RELEASE(key) glfwGetKey(window, GLFW_KEY_##key) == GLFW_RELEASE

class InputManager
{
private:
    Camera *camera;
    Scop *scop;

    float lastX;
    float lastY;
    float mixValue;

public:
    InputManager(GLFWwindow *window, Camera *camera, Scop *scop)
    {
        this->camera = camera;
        this->scop = scop;

        lastX =  800.0f / 2.0;
        lastY =  600.0 / 2.0;
        mixValue = 0;
        std::cout << "InputManager Constructor" << std::endl;

        glfwSetWindowUserPointer(window, this);
        this->set_callback_functions(window);
    };
    ~InputManager(){};

    void processInput(GLFWwindow *window)
    {
        if (GET_KEY_PRESS(UP))
        {
            mixValue += 0.0001f;
            if(mixValue >= 1.0f)
                mixValue = 1.0f;
            // cout << "mixvalue: " << mixValue << endl;
        }
        if (GET_KEY_PRESS(DOWN))
        {
            mixValue -= 0.0001f;
            if (mixValue <= 0.0f)
                mixValue = 0.0f;
        }
        if (GET_KEY_PRESS(W))
            camera->processKeyboard(FORWARD, Time::deltaTime);
        if (GET_KEY_PRESS(S))
            camera->processKeyboard(BACKWARD, Time::deltaTime);
        if (GET_KEY_PRESS(A))
            camera->processKeyboard(LEFT, Time::deltaTime);
        if (GET_KEY_PRESS(D))
            camera->processKeyboard(RIGHT, Time::deltaTime);
        if (GET_KEY_PRESS(SPACE))
            camera->processKeyboard(UP, Time::deltaTime);
        if (GET_KEY_PRESS(LEFT_SHIFT))
            camera->processKeyboard(DOWN, Time::deltaTime);
    };

    // void processInput(GLFWwindow *window, float deltaTime){};
    void set_callback_functions(GLFWwindow *window)
    {
        glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            InputManager *inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
            inputManager->scroll_callback(window, xoffset, yoffset);
        });
        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
        {
            InputManager *inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
            inputManager->mouse_callback(window, xpos, ypos);
        });
        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mode)
        {
            InputManager *inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
            inputManager->key_callback(window, key, scancode, action, mode);
        });
    }

    void scroll_callback(GLFWwindow *window, double horizontal, double vertcial)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            camera->processMouseScroll(vertcial, FOV_MODE);
        else
            camera->processMouseScroll(vertcial, SPEED_MODE);
        (void)horizontal;
    }

    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; 
        this->lastX = xpos;
        this->lastY = ypos;

        camera->processMouseMovement(xoffset, yoffset);
        (void)window;
    }


    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        
        if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
            scop->swap(1);
        if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
            scop->swap(-1);
        (void)scancode;
        (void)mode;
    }
};

#endif
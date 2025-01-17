#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" //load texture
#include <glad/glad.h> //load opengl function
#include <GLFW/glfw3.h> //window handling

#include "Shader.hpp"
#include "Object.hpp"
#include "ObjectLoader.hpp"
#include "LineDrawer.hpp"
#include "Time.hpp"
#include "utils.hpp"
#include "Camera.hpp"
#include "InputManager.hpp"
#include "Scop.hpp"
#include "Math.hpp"

#include <iostream>
#include <cstring>
#include <cmath>
#include <filesystem>
#include <algorithm>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define ASPECT_RATIO (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT

using std::string;

using std::cout;
using std::cerr;
using std::endl;

using std::sin;
using std::cos;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

GLFWwindow *createWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window" << std::endl;
        return NULL;
    }
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return window;
}

void print_welcome()
{
    std::cout << BRIGHT_YELLOW "\nWelcome to the Scop program!" RESET << std::endl;

    std::vector<std::string> obj_list = get_sorted_file_list("resources/", ".obj");

    std::cout << "Available objects:" << std::endl;
    for (size_t i = 0; i < obj_list.size(); i++)
    {
        std::cout << i << ": " << obj_list[i] << std::endl;
    }
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    print_welcome();

    GLFWwindow *window = createWindow();
    Time time;
    Camera camera = Camera(vec3(5,5,5), vec2(-135,-45));
    LineDrawer linedrawer = LineDrawer();
    Scop scop;
    InputManager inputManager = InputManager(window, &camera, &scop, &linedrawer);

    std::string filename = (argc >= 2 ? argv[1] : "teapot.obj");
    scop.load(filename);

    Shader defaultshader = Shader("shaders/default.vert", "shaders/default.frag");

    scop.setShader(&defaultshader);

    stbi_set_flip_vertically_on_load(true);

    // unsigned int texture1 = load_image("assets/earth.jpg", GL_REPEAT);
    unsigned int texture1 = load_image("assets/test.png", GL_REPEAT);

    //uncap frame rate to maximise fps
    glfwSwapInterval(0);
    glEnable(GL_DEPTH_TEST);

    linedrawer.add_axes();
    linedrawer.add_ygrid(5, 1);

    while (!glfwWindowShouldClose(window))
    {
        inputManager.processInput(window);
        time.update();
        camera.rotateAroundCenter(Time::deltaTime);
        glClearColor(0.7f, 0.7f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // create transformations
        mat4 model         = mat4(1.0f); // make sure to initialize matrix to identity matrix first
        mat4 view          = camera.GetViewMatrix();
        mat4 projection    = perspective(radians(camera.fov), ASPECT_RATIO, 0.0001f, 10000.0f);

        scop.draw((const GLfloat *)&model, (const GLfloat *)&view, (const GLfloat *)&projection);

        linedrawer.draw((const GLfloat *)&view, (const GLfloat *)&projection);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    std::cout << "Closing window\n";
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
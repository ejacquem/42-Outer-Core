#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" //load texture
#include "glad/include/glad/glad.h" //load opengl function
#include "glfw-3.4/include/GLFW/glfw3.h" //window handling

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.hpp"
#include "Object.hpp"
#include "ObjectLoader.hpp"
#include "LineDrawer.hpp"
#include "Time.hpp"
#include "utils.hpp"
#include "Camera.hpp"
#include "InputManager.hpp"
#include "Scop.hpp"

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

int main(int argc, char** argv)
{
    GLFWwindow *window = createWindow();
    Time time;
    Camera camera = Camera(glm::vec3(5,5,5), glm::vec2(-135,-45));
    std::string filename = (argc >= 2 ? argv[1] : "resources/teapot.obj");
    Scop scop(filename);
    InputManager inputManager = InputManager(window, &camera, &scop);

    Shader perspectiveshader = Shader("shaders/perspective.vert", "shaders/texture.frag");
    Shader lineshader = Shader("shaders/3d_line.vert", "shaders/3d_line.frag");
    Shader defaultshader = Shader("shaders/default.vert", "shaders/default.frag");

    scop.setShader(&defaultshader);

    stbi_set_flip_vertically_on_load(true);

    unsigned int texture1 = load_image("assets/wall.jpg", GL_RGB, GL_CLAMP_TO_EDGE);
    unsigned int texture2 = load_image("assets/awesomeface.png", GL_RGBA, GL_REPEAT);

    //uncap frame rate to maximise fps
    glfwSwapInterval(0);
    glEnable(GL_DEPTH_TEST);

    LineDrawer linedrawer = LineDrawer(&lineshader);
    linedrawer.add_axes();
    linedrawer.add_ygrid(5, 1);

    while (!glfwWindowShouldClose(window))
    {
        inputManager.processInput(window);
        time.update();
        glClearColor(0.7f, 0.7f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // create transformations
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = camera.GetViewMatrix();
        glm::mat4 projection    = glm::perspective(glm::radians(camera.fov), ASPECT_RATIO, 0.0001f, 100.0f);

        scop.draw(model, view, projection);

        linedrawer.draw(view, projection);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    std::cout << "Closing window\n";
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
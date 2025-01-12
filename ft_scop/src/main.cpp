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

#include <iostream>
#include <cstring>
#include <cmath>
#include <filesystem>
#include <algorithm>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define ASPECT_RATIO (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT
#define OBJ_PATH "resources/"

int g_obj_index = 0;
Object* g_obj;
ObjectLoader g_objLoader;

using std::string;

using std::cout;
using std::cerr;
using std::endl;

using std::sin;
using std::cos;

// Load an image from the given path and return the texture ID.
int load_image(const char *path, int srcDataFormat, int option1)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // load and generate the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, option1);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, option1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, srcDataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    return texture;
}

void swap_object(int direction)
{
    std::vector<std::string> obj_list = get_sorted_file_list(OBJ_PATH, ".obj");

    int len = obj_list.size();
    
    g_obj_index += direction;
    g_obj_index = (g_obj_index + len) % len;

    std::cout << "\nloading: " << obj_list[g_obj_index] << std::endl;
    delete g_obj;
    g_obj = g_objLoader.parse(OBJ_PATH + obj_list[g_obj_index]);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

float mixValue = 0;

void processInput(GLFWwindow *window)
{
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.0001f;
        if(mixValue >= 1.0f)
            mixValue = 1.0f;
        // cout << "mixvalue: " << mixValue << endl;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.0001f;
        if (mixValue <= 0.0f)
            mixValue = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->processKeyboard(FORWARD, Time::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->processKeyboard(BACKWARD, Time::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->processKeyboard(LEFT, Time::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->processKeyboard(RIGHT, Time::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->processKeyboard(UP, Time::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->processKeyboard(DOWN, Time::deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        swap_object(1);
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        swap_object(-1);
    (void)scancode;
    (void)mode;
}

float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    camera->processMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double horizontal, double vertcial)
{
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera->processMouseScroll(vertcial, SPEED_MODE);
    else
        camera->processMouseScroll(vertcial, FOV_MODE);
    (void)horizontal;
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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return window;
}

int main(int argc, char** argv)
{
    GLFWwindow *window = createWindow();
    Time time;
    Camera camera = Camera(glm::vec3(5,5,5), glm::vec2(-135,-45));

    glfwSetWindowUserPointer(window, &camera);

    // Shader blobshader = Shader("shaders/blob.vert", "shaders/default.frag");
    // Shader textshader = Shader("shaders/texture.vert", "shaders/texture.frag");
    // Shader transformshader = Shader("shaders/transform.vert", "shaders/texture.frag");
    Shader perspectiveshader = Shader("shaders/perspective.vert", "shaders/texture.frag");
    Shader lineshader = Shader("shaders/3d_line.vert", "shaders/3d_line.frag");
    Shader defaultshader = Shader("shaders/default.vert", "shaders/default.frag");

    stbi_set_flip_vertically_on_load(true);

    unsigned int texture1 = load_image("assets/wall.jpg", GL_RGB, GL_CLAMP_TO_EDGE);
    unsigned int texture2 = load_image("assets/awesomeface.png", GL_RGBA, GL_REPEAT);

    //uncap frame rate to maximise fps
    glfwSwapInterval(0);
    glEnable(GL_DEPTH_TEST);

    LineDrawer linedrawer = LineDrawer(lineshader);
    linedrawer.add_axes();
    // linedrawer.add_xgrid(5, 1);
    linedrawer.add_ygrid(5, 1);
    // linedrawer.add_zgrid(5, 1);

    std::string filename = argc >= 2 ? argv[1] : "resources/teapot.obj";
    g_obj_index = get_file_index(filename, OBJ_PATH);
    swap_object(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
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
        
        defaultshader.use();
        defaultshader.setMat4("model", model);
        defaultshader.setMat4("view", view);
        defaultshader.setMat4("projection", projection);

        g_obj->draw();

        lineshader.use();
        lineshader.setMat4("view", view);
        lineshader.setMat4("projection", projection);

        linedrawer.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    std::cout << "Closing window\n";
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
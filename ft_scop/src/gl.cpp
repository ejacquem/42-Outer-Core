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
#include <iostream>
#include <cstring>
#include <cmath>
#include <filesystem>
#include <algorithm>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define OBJ_PATH "resources/"

float g_camera_speed = 5; 

int g_obj_index = 0;
Object* g_obj;
ObjectLoader g_objLoader;

using std::string;

using std::cout;
using std::cerr;
using std::endl;

using std::sin;
using std::cos;

// return the list of .obj file in the resources/ folder
std::vector<std::string> get_sorted_obj_list()
{
    namespace fs = std::filesystem;
    std::string directory = OBJ_PATH;
    std::vector<std::string> obj_list;

    try {
        if (!fs::exists(directory) || !fs::is_directory(directory)) {
            std::cerr << "Directory does not exist or is not a directory." << std::endl;
            return obj_list;
        }

        for (const auto& entry : fs::directory_iterator(directory)) {
            if (fs::is_regular_file(entry)) {
                if (entry.path().extension() == ".obj")
                {
                    // std::cout << entry.path().filename() << std::endl;
                    obj_list.push_back(entry.path().filename());
                }
            }
        }
        std::sort(obj_list.begin(), obj_list.end());
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return obj_list;
}

//return the position of the file in alphabetical order
int get_obj_file_index(std::string& filename)
{
    std::vector<std::string> obj_list = get_sorted_obj_list();

    for (size_t i = 0; i < obj_list.size(); i++)
    {
        if(obj_list[i] == filename)
        {
            return i;
        }
    }
    return 0;
}


void swap_object(int direction)
{
    std::vector<std::string> obj_list = get_sorted_obj_list();

    int len = obj_list.size();
    
    g_obj_index += direction;
    g_obj_index = (g_obj_index + len) % len;

    std::cout << "\nloading: " << obj_list[g_obj_index] << std::endl;
    delete g_obj;
    g_obj = g_objLoader.parse(OBJ_PATH + obj_list[g_obj_index]);
}

int print_err(string msg)
{
    cerr << msg << endl;
    return -1;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

float mixValue = 0;
glm::vec3 cameraPos   = glm::vec3(1.0f, 1.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

int left;
int right;

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

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
    const float cameraSpeed = g_camera_speed * Time::deltaTime; // adjust accordingly
    glm::vec3 frontvec(cameraFront.x, 0, cameraFront.z);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * glm::normalize(frontvec);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * glm::normalize(frontvec);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraUp, frontvec));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraUp, frontvec));
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        if (left == 0)
            swap_object(-1);
        left = 1;        
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE)
    {
        left = 0;        
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        if (right == 0)
            swap_object(1);
        right = 1;        
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE)
    {
        right = 0;        
    }
}

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  90.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.9f)
        pitch = 89.9f;
    if(pitch < -89.9f)
        pitch = -89.9f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
    (void)window;
}

void scroll_callback(GLFWwindow* window, double horizontal, double vertcial)
{
    g_camera_speed *= pow(1.5, vertcial);
    if(g_camera_speed < 0.1)
        g_camera_speed = 0.1;
    (void)horizontal;
    (void)window;
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
        print_err("Failed to create GLFW window");
        return NULL;
    }
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        print_err("Failed to initialize GLAD");
        return NULL;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return window;
}

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

int main(int argc, char** argv)
{
    GLFWwindow *window = createWindow();
    Time time;

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

    std::string filename = argc >= 2 ? argv[1] : "resources/teapot2.obj";
    g_obj_index = get_obj_file_index(filename);


    g_obj = g_objLoader.parse(filename);

    while (!glfwWindowShouldClose(window))
    {
        time.update();
        processInput(window);
        glClearColor(0.7f, 0.7f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // create transformations
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        // model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        
        // move camera with input
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        projection = glm::perspective(glm::radians(fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 100.0f);

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
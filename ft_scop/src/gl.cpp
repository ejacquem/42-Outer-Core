#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" //load texture
#include "glad/include/glad/glad.h" //load opengl function
#include "glfw-3.4/include/GLFW/glfw3.h" //window handling

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.hpp"
#include "LineDrawer.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include "Time.hpp"
#include <iostream>
#include <cstring>
#include <cmath>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

using std::string;

using std::cout;
using std::cerr;
using std::endl;

using std::sin;
using std::cos;

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
    const float cameraSpeed = 5 * Time::deltaTime; // adjust accordingly
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

int main()
{
    GLFWwindow *window = createWindow();
    Time time;

    Shader blobshader = Shader("shaders/blob.vert", "shaders/default.frag");
    Shader textshader = Shader("shaders/texture.vert", "shaders/texture.frag");
    Shader transformshader = Shader("shaders/transform.vert", "shaders/texture.frag");
    Shader perspectiveshader = Shader("shaders/perspective.vert", "shaders/texture.frag");
    Shader lineshader = Shader("shaders/3d_line.vert", "shaders/3d_line.frag");

    stbi_set_flip_vertically_on_load(true);

    unsigned int texture1 = load_image("assets/wall.jpg", GL_RGB, GL_CLAMP_TO_EDGE);

    unsigned int texture2 = load_image("assets/awesomeface.png", GL_RGBA, GL_REPEAT);

    float vertices[] = {
        // positions          // colors          // texture coords
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // 0
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // 1
        0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // 2
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, // 3
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f, // 4
        0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f, // 5
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, // 6
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f  // 7
    };

    unsigned int indices[] = {
        // front face
        0, 1, 2, 0, 2, 3,
        // back face
        4, 5, 6, 4, 6, 7,
        // left face
        0, 3, 7, 0, 7, 4,
        // right face
        1, 5, 6, 1, 6, 2,
        // top face
        3, 2, 6, 3, 6, 7,
        // bottom face
        0, 1, 5, 0, 5, 4
    };

    VAO vao = VAO();
    VBO vbo = VBO(vertices, sizeof(vertices));
    EBO ebo = EBO(indices, sizeof(indices));

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    VAO::unbind();
    VBO::unbind();
    EBO::unbind();

    //uncap frame rate to maximise fps
    glfwSwapInterval(0);
    glEnable(GL_DEPTH_TEST);

    // glm::mat4 trans = glm::mat4(1.0f);
    // trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    // trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

    // glm::vec3 cubePositions[] = {
    //     glm::vec3( 0.0f,  5.0f,  0.0f), 
    //     glm::vec3( 2.0f,  5.0f, -15.0f), 
    //     glm::vec3(-1.5f, -2.2f, -2.5f),  
    //     glm::vec3(-3.8f, -2.0f, -12.3f),  
    //     glm::vec3( 2.4f, -0.4f, -3.5f),  
    //     glm::vec3(-1.7f,  3.0f, -7.5f),  
    //     glm::vec3( 1.3f, -2.0f, -2.5f),  
    //     glm::vec3( 1.5f,  2.0f, -2.5f), 
    //     glm::vec3( 1.5f,  0.2f, -1.5f), 
    //     glm::vec3(-1.3f,  1.0f, -1.5f)  
    // };

    LineDrawer linedrawer = LineDrawer(lineshader);
    linedrawer.add_axes();
    linedrawer.add_xgrid(5, 1);
    linedrawer.add_ygrid(5, 1);
    linedrawer.add_zgrid(5, 1);

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


        perspectiveshader.use();

        // create transformations
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        
        // move camera with input
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        projection = glm::perspective(glm::radians(fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 100.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));  
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        perspectiveshader.setMat4("model+", model);
        perspectiveshader.setMat4("view", view);
        perspectiveshader.setMat4("projection", projection);

        perspectiveshader.setInt("texture1", 0);
        perspectiveshader.setInt("texture2", 1);
        perspectiveshader.setFloat("mixValue", mixValue);

        vao.bind();

        // for(unsigned int i = 0; i < 10; i++)
        // {
        //     glm::mat4 model = glm::mat4(1.0f);
        //     model = glm::translate(model, cubePositions[i]);
        //     float angle = 20.0f * i; 
        //     model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //     model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));  
        //     perspectiveshader.setMat4("model", model);

        //     glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //     // glDrawArrays(GL_TRIANGLES, 0, 36);
        // }

        VAO::unbind();

        lineshader.use();

        lineshader.setMat4("view", view);
        lineshader.setMat4("projection", projection);

        linedrawer.draw();

        // glBindVertexArray(vao_line);

        // glDrawArrays(GL_LINES, 0, 6);

        // blobshader.use();
        // blobshader.setFloat("xOffset", sin(Time::currentTime*8)/16);
        // blobshader.setFloat("xOffset2", cos(Time::currentTime*8)/16);
        // blobshader.setFloat("yOffset", sin(Time::currentTime*8)/16);
        // glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        // cout << glfwGetTime() << endl;
        // cout << sin(Time::currentTime) << endl;

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();

    return 0;
}
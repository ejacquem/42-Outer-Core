#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" //load texture
#include "glad/include/glad/glad.h" //load opengl function
#include "glfw-3.4/include/GLFW/glfw3.h" //window handling

#include "Shader.cpp"
#include "Time.cpp"
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

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.001f;
        if(mixValue >= 1.0f)
            mixValue = 1.0f;
        cout << "mixvalue: " << mixValue << endl;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.001f;
        if (mixValue <= 0.0f)
            mixValue = 0.0f;
    }
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

    stbi_set_flip_vertically_on_load(true);

    unsigned int texture1 = load_image("wall.jpg", GL_RGB, GL_CLAMP_TO_EDGE);

    unsigned int texture2 = load_image("awesomeface.png", GL_RGBA, GL_REPEAT);

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f  // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // float vertices[] = {
    // // positions         // colors
    //  0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    // -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
    //  0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    // };
    // unsigned int VBO, VAO;
    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);
    // // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    // glBindVertexArray(VAO);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // // position attribute
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    // // color attribute
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    // glEnableVertexAttribArray(1);


    //uncap frame rate to maximise fps
    glfwSwapInterval(0);

    while (!glfwWindowShouldClose(window))
    {
        time.update();
        processInput(window);
        glClearColor(0.7f, 0.7f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);


        textshader.use();
        // textshader.setInt("texture1", 0);
        textshader.setInt("texture2", 1);
        textshader.setFloat("xOffset", sin(Time::currentTime*8)/16);
        textshader.setFloat("xOffset2", cos(Time::currentTime*8)/16);
        textshader.setFloat("yOffset2", sin(Time::currentTime*8)/16);
        textshader.setFloat("yOffset", cos(Time::currentTime*8)/16);
        textshader.setFloat("mixValue", mixValue);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
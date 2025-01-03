#ifndef VBO_HPP
#define VBO_HPP

#include "glad/include/glad/glad.h"

//Vertex Buffer Object
class VBO
{
public:
    GLuint id;
    VBO(GLfloat* vertices, GLsizeiptr size);

    void bind();
    static void unbind();
    void deleteVBO();
};

#endif
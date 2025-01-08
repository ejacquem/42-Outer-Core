#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>
#include "Shader.hpp"
#include "scop.hpp"

// template <typename T>
class Object
{
private:
    GLuint vao, vbo, ebo;
    std::vector<Vertex> &vertices;
    std::vector<Indice> &indices;

public:
    void draw();
    Object(std::vector<Vertex> &vertices, std::vector<Indice> &indices);
    ~Object();
};

#endif
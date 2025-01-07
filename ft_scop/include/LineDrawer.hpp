#ifndef LINEDRAWER_HPP
#define LINEDRAWER_HPP

#include <vector>
#include "Shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"

struct Vertex
{
    float x, y, z;
    float r, g, b;
};

//this class will contain lines and draw them
// - add_line: add a line to the list of lines
// - draw: draw all the lines
// - toggle: toggle the visibility of the lines
// - clear: clear all the lines
// - add_axes: add the x, y, z axes
// - add_xgrid(n): add a grid of n lines in the yz plane
// - add_ygrid(n): add a grid of n lines in the xz plane
// - add_zgrid(n): add a grid of n lines in the xy plane
class LineDrawer
{
private:
    //vector of lines
    std::vector<Vertex> lines_vertex;
    Shader shader;
    GLuint vao, vbo;
    // VAO vao;
    // VBO vbo;

public:
    LineDrawer(Shader shader);
    ~LineDrawer();

    //add a line to the list of lines
    void add_line(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b);

    // void add_line(const Line3D line);

    //draw all the lines
    void draw();

    //toggle the visibility of the lines
    void toggle();

    //clear all the lines
    void clear();

    //add the x, y, z axes
    void add_axes();

    //add a grid of n lines in the yz plane
    void add_xgrid(int n);

    //add a grid of n lines in the xz plane
    void add_ygrid(int n);

    //add a grid of n lines in the xy plane
    void add_zgrid(int n);
};

#endif
#include "LineDrawer.hpp"

LineDrawer::LineDrawer(Shader shader) : shader(shader) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);  // Initially empty

    // Set up position attributes (x1, y1, z1, x2, y2, z2)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set up color attributes int (color)                          //offset of 3 floats
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

LineDrawer::~LineDrawer(){}

//add a line to the list of lines
void LineDrawer::add_line(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b) {
    lines_vertex.push_back({x1, y1, z1, r, g, b});
    lines_vertex.push_back({x2, y2, z2, r, g, b});
}

//draw all the lines
void LineDrawer::draw()
{
    if (lines_vertex.empty()) return;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, lines_vertex.size() * sizeof(Vertex), lines_vertex.data());

    shader.use();

    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, lines_vertex.size());  // Each line has two vertices
    // std::cout << "Drawing " << lines_vertex.size() << " lines_vertex" << std::endl;

    glBindVertexArray(0);
}

//toggle the visibility of the lines
void LineDrawer::toggle(){}

//clear all the lines
void LineDrawer::clear(){}

//add the x, y, z axes
void LineDrawer::add_axes(){}

//add a grid of n lines in the yz plane
void LineDrawer::add_xgrid(int n){(void)n;}

//add a grid of n lines in the xz plane
void LineDrawer::add_ygrid(int n){(void)n;}

//add a grid of n lines in the xy plane
void LineDrawer::add_zgrid(int n){(void)n;}

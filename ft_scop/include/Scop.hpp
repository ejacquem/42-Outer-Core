#ifndef SCOP_HPP
#define SCOP_HPP

#include "Camera.hpp"
#include "Shader.hpp"
#include "Object.hpp"
#include "ObjectLoader.hpp"
#include "LineDrawer.hpp"

class Scop
{
public:
    Scop();
    ~Scop();

    // process input/move the camera/swaps objects
    void update();
    // draw the scene
    void draw();
private:
    Camera *camera;
    Object *object;
    LineDrawer linedrawer;

};

#endif
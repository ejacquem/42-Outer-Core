#ifndef SCOP_HPP
#define SCOP_HPP

#include "Camera.hpp"
#include "Shader.hpp"
#include "Object.hpp"
#include "ObjectLoader.hpp"
#include "LineDrawer.hpp"
#include "utils.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define OBJ_PATH "resources/"

class Scop
{

public:
    Scop();
    ~Scop();

    void swap(int direction);
    void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
    void load(const std::string &filename);
    void setShader(Shader *shader);

private:
    Object *object;
    Shader *shader;
    ObjectLoader objLoader;

    int obj_index;

};

#endif
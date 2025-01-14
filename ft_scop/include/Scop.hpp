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
    Scop()
    {
        obj_index = 0;
        object = nullptr;
    };
    ~Scop()
    {
        if (object)
            delete object;
    };

    void swap(int direction)
    {
        std::vector<std::string> obj_list = get_sorted_file_list(OBJ_PATH, ".obj");

        int len = obj_list.size();
        
        obj_index += direction;
        obj_index = (obj_index + len) % len;

        load(OBJ_PATH + obj_list[obj_index]);
    };

    void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
    {
        if(object == NULL)
            return;
        shader->use();
        shader->setMat4("model", model);
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);

        object->draw();
    };

    void load(const std::string &filename)
    {
        std::cout << "\nloading: " << filename << std::endl;
        if (object)
            delete object;
        object = objLoader.parse(filename);
    };

    void setShader(Shader *shader)
    {
        this->shader = shader;
    };

private:
    Object *object;
    Shader *shader;
    ObjectLoader objLoader;

    int obj_index;

};

#endif
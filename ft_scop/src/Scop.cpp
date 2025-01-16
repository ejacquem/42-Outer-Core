#include "Scop.hpp"

Scop::Scop()
{
    obj_index = -1;
    object = nullptr;
}

Scop::~Scop()
{
    if (object)
        delete object;
}

void Scop::swap(int direction)
{
    std::vector<std::string> obj_list = get_sorted_file_list(OBJ_PATH, ".obj");

    int len = obj_list.size();
    
    obj_index += direction;
    obj_index = (obj_index + len) % len;

    load(OBJ_PATH + obj_list[obj_index]);
}

void Scop::draw(const GLfloat *model, const GLfloat *view, const GLfloat *projection)
{
    if(object == NULL)
        return;
    shader->use();
    shader->setMat4("model", model);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    object->draw();
}

void Scop::load(const std::string &filename)
{
    std::cout << "\nloading: " << filename << std::endl;
    if (obj_index == -1)
    {
        obj_index = get_file_index(filename, OBJ_PATH);
        std::cout << "obj_index: " << obj_index << std::endl;
    }
    if (object)
        delete object;
    object = objLoader.parse(filename);
}

void Scop::setShader(Shader *shader)
{
    this->shader = shader;
}
#ifndef OBJECTLOADER_HPP
#define OBJECTLOADER_HPP

#include <vector>
#include <deque>
#include "Shader.hpp"
#include "scop.hpp"
#include "Object.hpp"
#include "Time.hpp"

//load object data from file
class ObjectLoader
{
private:
    std::vector<Vertex> vertices_buffer;
    std::vector<Indice> indices_buffer;
    void parseLine(const std::string& line);
    void parseIndice(std::istringstream& stream);

public:
    Object* parse(const std::string& filePath);
    // Object parseFast(const std::string &filePath);
    ObjectLoader();
    ~ObjectLoader();
};

#endif
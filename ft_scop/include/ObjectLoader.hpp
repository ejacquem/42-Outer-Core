#ifndef OBJECTLOADER_HPP
#define OBJECTLOADER_HPP

#include <vector>
#include "Shader.hpp"
#include "scop.hpp"
#include "Object.hpp"

//load object data from file
class ObjectLoader
{
private:
    std::vector<Vertex> vertices_buffer;
    std::vector<Indice> indices_buffer;
    void parseLine(const std::string& line);
    void parseVertex(const std::string& line);

public:
    Object parse(const std::string& filePath);
    ObjectLoader();
    ~ObjectLoader();
};

#endif